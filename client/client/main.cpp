// placeholder

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "MFRC522.h"
#include "bcm2835.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PIN RPI_GPIO_P1_08
#define DEFAULT_BUFLEN 512

void delay(int ms) {
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void bytes2hex(char Hex[/* 2*Sz */], unsigned char Bytes[ /* Sz */], size_t Sz)
{
    char const static d[] = "0123456789abcdef";
    for (size_t i = 0; i < Sz; i++)
        Hex[i * 2 + 0] = d[Bytes[i] / 16], Hex[i * 2 + 1] = d[Bytes[i] % 16];
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    printf("serverSocket: %d\n", serverSocket);

    struct sockaddr_in server;
    unsigned long addr;

    memset(&server, 0, sizeof(server));
    addr = inet_addr("192.168.2.215");
    memcpy((char*)&server.sin_addr, &addr, sizeof(addr));
    server.sin_family = AF_INET;
    server.sin_port = htons(8150);

    int connectResult = connect(serverSocket, (struct sockaddr*)&server, sizeof(server));
    printf("connectResult: %d\n", connectResult);

    MFRC522 mfrc;

    mfrc.PCD_Init();

    bcm2835_init();
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(PIN, HIGH);

    while(1)
    {
        // Look for a card
        if(!mfrc.PICC_IsNewCardPresent())
            continue;

        if( !mfrc.PICC_ReadCardSerial())
            continue;

        byte cardNumber[4] = {mfrc.uid.uidByte[0], mfrc.uid.uidByte[1], mfrc.uid.uidByte[2], mfrc.uid.uidByte[3]};

        //unsigned char bytes[] = { 0xaa,0xbb,0xcc,0x11,0x22 };
        char hex[2 * sizeof(cardNumber) + 1 /*for the '\0' */];
        hex[sizeof(hex) - 1] = '\0';
        bytes2hex(hex, cardNumber, sizeof(cardNumber));

        // Print UID
        /*for(byte i = 0; i < mfrc.uid.size; ++i)
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
        printf("\n");*/

        //char* sendBuf = "a";
        int sendResult = send(serverSocket, hex, (int)strlen(hex), 0);
        printf("sendResult: %d\n", sendResult);
        printf("sending: %s\n", hex);

        delay(5000);

        /*bcm2835_gpio_write(PIN, LOW);
        bcm2835_delay(5000);
        bcm2835_gpio_write(PIN, HIGH);
        bcm2835_delay(5000);*/

    }
    return 0;
}
