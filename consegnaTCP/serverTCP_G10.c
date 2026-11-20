#ifdef WIN32
#include <winsock2.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N_BUFFER 12 //Buffer for numbers
#define WELPORT 7777 //Default port protocol number
#define QLEN 2 //Queue max lenght

void clearWinSock();

int main(){

    // Winsock initialization
    #if defined WIN32
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
            printf("Errore allo startup\n");
            return -1;
        }
    #endif

    // Welcome socket creation
    int welSocket;
    welSocket = socket(PF_INET, SOCK_STREAM,0);

    if (welSocket < 0){
        printf("Errore nell'inizializzazione della socket\n");
        clearWinSock();
        return -1;
    }

    //Binding address and port number to Welcome Socket
    struct sockaddr_in wSad;

    wSad.sin_family = AF_INET;
    wSad.sin_addr.s_addr = inet_addr("127.0.0.1");
    wSad.sin_port= htons(WELPORT);

    if(bind(welSocket,(struct sockaddr *)&wSad,sizeof(wSad))<0){
        printf("Errore nel bind della socket\n");
        closesocket(welSocket);
        clearWinSock();
        return -1;
    }

    //Setting Welcome Socket to listen
    if (listen(welSocket,QLEN)<0){
        printf("Errore di listen()\n");
        closesocket(welSocket);
        clearWinSock();
        return -1;
    }
    
    //Accepting new connection
    struct sockaddr_in cSad;
    int cliSocket;
    int cliAddrLen;

    printf("Aspettando connessione...\n");
    while (1){
        cliAddrLen = sizeof(cSad);
        if ((cliSocket = accept(welSocket,(struct sockaddr*) &cSad, &cliAddrLen))<0){
            printf("Errore connesione\n");
            closesocket(welSocket);
            clearWinSock();
            return -1;
        }
        
        printf("Connesso al cliente di IP: %s\n",inet_ntoa(cSad.sin_addr));

        //Sending string check for succesful connection
        char* stringa = "connessione avvenuta";

        if(send(cliSocket, stringa ,strlen(stringa),0) <= 0){
            printf("Errore all'invio\n");
            closesocket(welSocket);
            clearWinSock();
            return -1;
        }

        int res;
        char n1[N_BUFFER], n2[N_BUFFER], n3[N_BUFFER];
        do{
            res=0;
            int byteRcvd;

            //Reciving numbers from client
            byteRcvd = recv(cliSocket, n1, sizeof(n1)-1 ,0);
            if (byteRcvd <= 0){
                printf("Errore nella ricezione\n");
                closesocket(welSocket);
                clearWinSock();
                return -1;
            }
            n1[byteRcvd] = '\0';

            byteRcvd = recv(cliSocket, n2, sizeof(n2)-1 ,0);
            if (byteRcvd <= 0){
                printf("Errore nella ricezione\n");
                closesocket(welSocket);
                clearWinSock();
                return -1;
            }
            n2[byteRcvd] = '\0';

            byteRcvd = recv(cliSocket, n3, sizeof(n3)-1 ,0);
            if (byteRcvd <= 0){
                printf("Errore nella ricezione\n");
                closesocket(welSocket);
                clearWinSock();
                return -1;
            }
            n3[byteRcvd] = '\0';

            int num1,num2,num3;
            num1 = atoi(n1);
            num2 = atoi(n2);
            num3 = atoi(n3);

            res = num1+num2+num3;
            printf("La somma di %d+%d+%d = %d\n",num1,num2,num3,res);

            char result[N_BUFFER];

            if (res != 100){
                snprintf(result,sizeof(result),"%d", res);
            }else{
                strcpy(result,"QUIT");
            }

            if(send(cliSocket, result ,strlen(result),0) <= 0){
                printf("Errore all'invio\n");
                closesocket(welSocket);
                clearWinSock();
                return -1;
            }
        } while (res != 100);
        
    }
}

void clearWinSock(){
    #if defined WIN32
        WSACleanup();
    #endif
}