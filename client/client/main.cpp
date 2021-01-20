#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void delay(int ms){
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms*1000);
#endif
}

#include "MFRC522.h"
#include "bcm2835.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PIN RPI_GPIO_P1_08

int main()
{
    int socketCDI = socket(AF_INET, SOCK_STREAM, 0);
    printf("socketCDI: %d\n", socketCDI);

    struct sockaddr_in server;
    unsigned long addr;

    memset(&server, 0, sizeof(server));
    addr = inet_addr("192.168.2.215");
    memcpy((char*)&server.sin_addr, &addr, sizeof(addr));
    server.sin_family = AF_INET;
    server.sin_port = htons(8150);

    int connectResult = connect(socketCDI, (struct sockaddr*)&server, sizeof(server));
    printf("serverConnection: %d\n", connectResult);

    MFRC522 mfrc;

    mfrc.PCD_Init();

    bcm2835_init();
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

    while(1)
    {
        // Look for a card
        if(!mfrc.PICC_IsNewCardPresent())
            continue;

        if( !mfrc.PICC_ReadCardSerial())
            continue;

        // Print UID
        for(byte i = 0; i < mfrc.uid.size; ++i)
        {
            if(mfrc.uid.uidByte[i] < 0x10)
            {
	            printf(" 0");
	            printf("%X",mfrc.uid.uidByte[i]);
            }
            else
            {
	            printf(" ");
	            printf("%X", mfrc.uid.uidByte[i]);
            }     
        }
        printf("\n");

    /*bcm2835_gpio_write(PIN, LOW);
    bcm2835_delay(5000);
    bcm2835_gpio_write(PIN, HIGH);
    bcm2835_delay(5000);*/

    }
    return 0;
}
