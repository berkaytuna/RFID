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
#include <iostream>
#include <arpa/inet.h>
#include <random>
#include <sstream>
#include "request.h"
#include <iomanip>

#define PIN RPI_GPIO_P1_08
#define DEFAULT_BUFLEN 512

//namespace uuid {
    /*static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);*/
    string generate_uuid() /*generate_uuid_v4()*/ 
    {
        static std::random_device              rd;
        static std::mt19937                    gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);
        static std::uniform_int_distribution<> dis2(8, 11);

        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }
//}

void delay(int ms) {
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

string hex2str(byte* data, int len)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i(0); i < len; ++i)
        ss << std::setw(2) << std::setfill('0') << (int)data[i];

    return ss.str();
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

    delay(500);

    int local_port = 0;
    socklen_t addrlen = sizeof(server);
    if (getsockname(serverSocket, (struct sockaddr*)&server, &addrlen) == 0 &&
        server.sin_family == AF_INET && addrlen == sizeof(server))
    {
        local_port = ntohs(server.sin_port);
    }
    cout << local_port << endl;

    // setDeviceCommunicationportRequest
    string endpointStr = std::to_string(local_port);
    const char* charPtr = endpointStr.c_str();  //use char const* as target type
    char* deviceCommunicationPort = new char[endpointStr.size() + 1];
    std::copy(endpointStr.begin(), endpointStr.end(), deviceCommunicationPort);
    deviceCommunicationPort[endpointStr.size()] = '\0'; // don't forget the terminating 0

    //char deviceCommunicationPort[2] = "6";
    char deviceCommunicationIp[14] = "192.168.2.142";
    char deviceIp[14] = "192.168.2.142";
    char devicePort[2] = "1";
    string uuidStr = generate_uuid();
    char uuid[37] = { };
    for (int i = 0; i < 37; i++)
    {
        uuid[i] = uuidStr[i];
    }
    char commandTypeName[61] = "CDI_Shared_PCL.DataObjects.SetDeviceCommunicationPortRequest";
    char isResponse[6] = "false";
    char isNotification[6] = "false";

    vector<char*> params {deviceCommunicationPort, deviceCommunicationIp, deviceIp, devicePort, uuid, commandTypeName, isResponse, isNotification };
    vector<int> sizes { (int)endpointStr.size() + 1, 14, 14, 2, 37, 61, 6, 6 };

    setDeviceCommunicationPortRequest setDeviceCommunicationPortRequest;
    vector<byte> command_vec = setDeviceCommunicationPortRequest.createRequest(params, sizes);
    byte* command = &command_vec[0];

    for (int i = 0; i < command_vec.size(); i++)
    {
        cout << command[i];
    }
    cout << endl;

    int sendResult = send(serverSocket, command, command_vec.size() /*(int)strlen(hex)*/, 0);
    printf("sendResult: %d\n", sendResult);



    delete[] deviceCommunicationPort;



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

        //byte cardNumber[4] = {mfrc.uid.uidByte[0], mfrc.uid.uidByte[1], mfrc.uid.uidByte[2], mfrc.uid.uidByte[3]};

        string cardNumberStr = hex2str(mfrc.uid.uidByte, 4);
        cout << cardNumberStr << endl;

        const char* charPtr = cardNumberStr.c_str();  //use char const* as target type
        char* cardNumber = new char[cardNumberStr.size() + 1];
        std::copy(cardNumberStr.begin(), cardNumberStr.end(), cardNumber);
        cardNumber[cardNumberStr.size()] = '\0'; // don't forget the terminating 0

        //unsigned char bytes[] = { 0xaa,0xbb,0xcc,0x11,0x22 };
        //char hex[2 * sizeof(cardNumber) + 1 /*for the '\0' */];
        //hex[sizeof(hex) - 1] = '\0';
        //bytes2hex(hex, cardNumber, sizeof(cardNumber));

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

        // accessRequest
        //char cardNumber[9] = cardNumberByteArray;
        char lockerSegmentInfo[5] = "null";
        char deviceIp[14] = "192.168.2.142";
        char devicePort[2] = "1";
        string uuidStr = generate_uuid();
        char uuid[37] = { };
        for (int i = 0; i < 37; i++)
        {
            uuid[i] = uuidStr[i];
        }
        char commandTypeName[41] = "CDI_Shared_PCL.DataObjects.AccessRequest";
        char isResponse[6] = "false";
        char isNotification[6] = "false";

        vector<char*> params{ cardNumber, lockerSegmentInfo, deviceIp, devicePort, uuid, commandTypeName, isResponse, isNotification };
        vector<int> sizes{ 9, 5, 14, 2, 37, 41, 6, 6 };

        accessRequest accessRequest;
        vector<byte> command_vec = accessRequest.createRequest(params, sizes);
        byte* command2 = &command_vec[0];

        for (int i = 0; i < command_vec.size(); i++)
        {
            cout << command2[i];
        }
        cout << endl;

        int sendResult = send(serverSocket, command2, command_vec.size() /*(int)strlen(hex)*/, 0);
        printf("sendResult: %d\n", sendResult);
        //printf("sending: %s\n", hex);



        delete[] cardNumber;



        // receiveMessage
        char recvBuf[DEFAULT_BUFLEN];
        int recvBufLen = DEFAULT_BUFLEN;
        int recvResult = recv(serverSocket, recvBuf, recvBufLen, 0);
        printf("recvResult: %d\n", recvResult);
        printf("%s\n", recvBuf);

        /*for (int i; i < sizeof(hex) / sizeof(hex[0]); i++)
        {
            printf("first byte: %d\n", hex[i]);
        }*/

        //delay(5000);

        if (recvBuf[22] == 't')
        {
            bcm2835_gpio_write(PIN, LOW);
            bcm2835_delay(5000);
            bcm2835_gpio_write(PIN, HIGH);
            bcm2835_delay(5000);
        }
        else
        {
            delay(5000);
        }

    }
    return 0;
}
