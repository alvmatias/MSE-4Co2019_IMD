/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "mylcd.h"
/*==================[macros and definitions]=================================*/
#define BUFFER_LENGTH 81               ///< The buffer length (crude but fine)
/*==================[internal data declaration]==============================*/
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
char buffer[3];
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int main()
{
    int ret, fd;
    char stringToSend[BUFFER_LENGTH] = "Hola Mundo";
    char ch;

    printf("Starting device test code example...\n");
    /* Abrimos el dispositivo */
    fd = open("/dev/LCD_HD44780", O_RDWR);             
    if (fd < 0)
    {
        perror("Failed to open the device...");
        return errno;
    }

    /* Escribimos en el dispositivo */
    ret = write(fd, stringToSend, strlen(stringToSend)); 

    if (ret < 0)
    {
        perror("Failed to write the message to the device.");
        return errno;
    }
	
    printf("Reading from the device...\n");
    /* Leemos 3 caracteresd el dispositivo */
    ret = read(fd, receive, 3);       
    if (ret < 0){
        perror("Failed to read the message from the device.");
        return errno;
    }
    printf("The received message is: [%s]\n", receive);

    printf("Reading from the device...\n");
    /* Leemos 5 caracteresd el dispositivo */
    ret = read(fd, receive, 5);       
    if (ret < 0){
        perror("Failed to read the message from the device.");
        return errno;
    }
    printf("The received message is: [%s]\n", receive);
    
    /* Menu */
    while(1)
    {
        printf("Ingresa:\n");

        printf("1: SetChar\n");
        printf("2: GetChar\n");

        printf("3: SetPosition\n");
        printf("4: GetPosition\n");

        printf("5: Reset\n");
        printf("6: Clear\n");
        printf("7: Home\n");

        printf("8: SetBacklight\n");
        printf("9: GetBacklight\n");

        printf("A: SetCursor\n");
        printf("B: GetCursor\n");

        printf("C: SetBlink\n");
        printf("D: GetBlink\n");

        printf("F: Finalizar\n");

        scanf("%c", &ch);    
	
        /* Ejecutamos la opcion enviada por el usuario */
        switch(ch)
        {
            case '1':
		scanf("%c", &ch);
                buffer[0] = ch;
                if(0 > ioctl(fd, LCD_IOCTL_SETCHAR, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;
            case '2':
                if(0 > ioctl(fd, LCD_IOCTL_GETCHAR, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
                else
                {
                    printf("Char actual: %c\n\n", buffer[0]);
                }
            break;
            case '3':
		scanf("%d", &ch);
                buffer[0] = ch;
		scanf("%d", &ch);
                buffer[1] = ch;
                if(0 > ioctl(fd, LCD_IOCTL_SETPOSITION, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;

            case '4':
                if(0 > ioctl(fd, LCD_IOCTL_GETPOSITION, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
                else
                {
                    printf("Pos actual: %d %d\n\n", buffer[0], buffer[1]);
                }
            break;

            case '5':
		scanf("%c", &ch);
                buffer[0] = ch;

                if(0 > ioctl(fd, LCD_IOCTL_RESET, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;

            case '6':
		scanf("%c", &ch);
                buffer[0] = ch;
                if(0 > ioctl(fd, LCD_IOCTL_CLEAR, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;

            case '7':
		scanf("%c", &ch);
                buffer[0] = ch;
                if(0 > ioctl(fd, LCD_IOCTL_HOME, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;

            case '8':
		scanf("%c", &ch);
                buffer[0] = ch;
                if(0 > ioctl(fd, LCD_IOCTL_SETBACKLIGHT, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;

            case '9':
                if(0 > ioctl(fd, LCD_IOCTL_GETBACKLIGHT, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
                else
                {
                    printf("Backlight actual: %c\n\n", buffer[0]);
                }
            break;

            case 'A':
                buffer[0] = '1';
                if(0 > ioctl(fd, LCD_IOCTL_SETCURSOR, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;

            case 'B':
                if(0 > ioctl(fd, LCD_IOCTL_GETCURSOR, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
                else
                {
                    printf("Cursor actual: %c\n\n", buffer[0]);
                }
            break;

            case 'C':
		scanf("%c", &ch);
                buffer[0] = ch;
                if(0 > ioctl(fd, LCD_IOCTL_SETBLINK, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
            break;

            case 'D':
                if(0 > ioctl(fd, LCD_IOCTL_GETBLINK, buffer))
                {
                    perror("Failed to ioctl\n");
                    return errno;
                }
                else
                {
                    printf("Blink actual: %c\n\n", buffer[0]);
                }
            break;

            case 'F':
                printf("End of the program\n\n");
                close(fd); 
                return 0;
            break;
        }

    }

    return 0;
}
/*==================[end of file]============================================*/
