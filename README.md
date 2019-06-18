# HD4470 Linux I2C Driver

## Documentacion

https://www.sparkfun.com/datasheets/LCD/HD44780.pdf

## Comandos implementados

1. Inicializacion correcta del lcd
2. Escritura de datos en el lcd
3. Lectura de la DDRAM del lcd
4. Seteo y obtencion de posicion de cursor
5. Seteo y obtencion de estado de cursor
6. Seteo y obtencion de estado de blinkeo del cursor
7. Seteo y obtencion de estado de luz de fondo del lcd
8. Escritura y lectura de un solo caracter en posicion actual
9. Reset
10. Clear
11. Home

## Contenido del repositorio

1. Codigo fuente del driver desarrollado y su header
	mylcd.c
	mylcd.h
2. Codigo fuente de la aplicacion de usuario
	lcdExample.c
3. Device tree "custom" para el driver
	am335x-customboneblack.dts
4. Makefile para compilar el driver
	Makefile
5. Se obtiene el archivo zimage

## Instrucciones

##### Compilacion del kernel

1. En linux-kernel-labs/src/linux:
	export ARCH=arm
  	export CROSS_COMPILE=arm-linux-gnueabi-
	make -j4 o -j8 dependiendo de la cantidad de hilos del procesador

##### Compilacion del device tree "custom"

1. Copiar el archivo am335x-customboneblack.dts en linux-kernel-labs/src/linux/arch/arm/boot/dts mediante
	sudo cp am335x-customboneblack.dtb /var/lib/tftpboot
2. En linux-kernel-labs/src/linux:
	make dtbs
3. Se obtiene el archivo am335x-customboneblack.dtb


##### Cargar el device tree y el zimage a la BBB

1. En /var/lib/tftpboot/ copiar los archivos am335x-customboneblack.dtb y zimage.
2. Conectar la BBB y abrir una sesion de una terminal
3. Configurar el ethernet de tu computadora desde una terminal en linux:
	sudo ifconfig enp2s0f5 192.168.0.1/24
4. Ejecutar los siguientes comandos, modificando las ip segun conveniencia:
	setenv ipaddr 192.168.0.100
	setenv serverip 192.168.0.1
	setenv bootargs root=/dev/nfs rw ip=192.168.0.100:::::eth0 console=ttyO0,115200n8 g_ether.dev_addr=f8:dc:7a:00:00:02 g_ether.host_addr=80:ee:73:4b:e3:ec nfsroot=192.168.0.1:/home/matias/linux-kernel-labs/modules/nfsroot,nfsvers=3
	tftp 0x81000000 zImage
	tftp 0x82000000 am335x-customboneblack.dtb
	bootz 0x81000000 - 0x82000000
5. Iniciar sesion con el usuario root

##### Instalar el modulo y probarlo con la app de usuario
1. Ejecutar en la BBB
	cd mylcd
	insmod mylcd.ko
2. Compilar en una terminal de linux la app de usuario
	 arm-linux-gnueabi-gcc -o lcdExample lcdExample.c
3. Ejecutar en la BBB
	./lcdExample
4. Al finalizar ejecutar en la BBB
	rmmod mylcd.ko

## App de Usuario

1. Escribe Hola Mundo en el lcd
2. Lee 3 bytes de la ddram del lcd
3. Lee 5 bytes de la ddram del lcd
4. Comienza un menu de usuario con las siguientes funciones
	1. SetChar - Espera que ingreses el char
	2. GetChar - Informa el char
	3. SetPosition - Espera 2 numeros para la posicion
	4. GetPosition - Informa la posicion
	5. Reset
	6. Clear
	7. Home
	8. SetBacklight - Espera un 0 o un 1 para apagar o prender la luz de fondo
	9. GetBackLight - Informa el estado de la luz de fondo
	A. SetCursor - Espera un 0 o un 1 para apagar o prender el cursor
	B. GetCursor - Informa el estado del cursor
	C. SetBlink - Espera un 0 o un 1 para apagar o prender el blinking del cursor
	D. GetBlink - Informa el estado del cursor
	F. Finaliza el programa

## Implementando una app de usuario propia

En primer lugar se debe abrir el dispositivo de la siguiente manera:
> open("/dev/LCD_HD44780", O_RDWR); 

#### Importante: Abrirlo para lectura y escritura.

Luego se puede escribir con la siguiente system call:
>  write(fd, stringToSend, strlen(stringToSend)); 

#### Importante: Hasta 80 caracteres van a ser tenidos en cuenta por el write. Ademas siempre se escribe desde la posición actual del cursor, esto quiere decir que si primero escribimos "Hola" y en una segunda llamada "Mundo", veremos en el lcd "Hola Mundo".

Para leer se utiliza la siguiente system call:
> read(fd, recvString, 3); 
#### Importante: SIEMPRE lee desde el principio(cursor en 0,0), lo indicado por el usuario o hasta un máximo de 80 caracteres.

Para ejecutar otro tipo de comandos usamos la siguiente system call:
> ioctl(fd, funcion, buffer)
#### Importante: En este caso función es una de las macros definidas en el header del driver:
	LCD_IOCTL_GETCHAR -> Obtener caracter actual     
	LCD_IOCTL_SETCHAR -> Imprimir un caracter en la posicion actual
	LCD_IOCTL_GETPOSITION -> Obtener posicion cursor
	LCD_IOCTL_SETPOSITION -> Setear posicion cursor
	LCD_IOCTL_RESET	-> Resetear el lcd
	LCD_IOCTL_HOME	-> Volver el cursor al 0,0
	LCD_IOCTL_SETBACKLIGHT	-> Prender o apgar el color de fondo
	LCD_IOCTL_GETBACKLIGHT	-> Obtener el estado del color del fondo
	LCD_IOCTL_SETCURSOR -> Prender o apagar el cursor
	LCD_IOCTL_GETCURSOR	-> Obtener el estado del cursor
	LCD_IOCTL_SETBLINK	-> Prender o apagar el blinking del cursor
	LCD_IOCTL_GETBLINK -> Obtener el estado del blinking  del cursor
	LCD_IOCTL_CLEAR -> Limpiar la pantalla sin volver al inicio el cursor

Por ultimo, para liberar el dispositivo, se usa la system call:
> close(fd); 
