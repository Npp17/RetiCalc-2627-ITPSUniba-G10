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
#define WELPORT 7777 //Default port protocol number
#define BUFFER_SIZE 512 // Buffer for string
#define N_BUFFER 12 //Buffer for numbers

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

    // Client socket creation
    int cliSocket;
    cliSocket = socket(PF_INET, SOCK_STREAM,0);

    if (cliSocket < 0){
        printf("Errore nell'inizializzazione della socket\n\n");
        clearWinSock();
        return -1;
    }

    // Connection to server
    struct sockaddr_in wSad;

    wSad.sin_family = AF_INET;
    wSad.sin_addr.s_addr = inet_addr("127.0.0.1");
    wSad.sin_port= htons(WELPORT);

    if(connect(cliSocket,(struct sockaddr*) &wSad, sizeof(wSad)) < 0){
        printf("Errore nella connessione\n");
        closesocket(cliSocket);
        clearWinSock();
        return -1;
    }

    //Reception of connection succesful message
    char buffer[BUFFER_SIZE];
    int byteRcvd;

    byteRcvd = recv(cliSocket, buffer, BUFFER_SIZE-1 ,0);
    if (byteRcvd <= 0){
        printf("Errore nella ricezione\n");
        closesocket(cliSocket);
        clearWinSock();
        return -1;
    }
    buffer[byteRcvd]='\0';
    printf("%s\n",buffer);

    int num1,num2,num3;
    char n1[N_BUFFER],n2[N_BUFFER] ,n3[N_BUFFER];
    do
    {
        
        printf("Inserire tre interi\n");
        scanf("%d",&num1);
        scanf("%d",&num2);
        scanf("%d",&num3);

        //Conversion of num from int to string
        snprintf(n1,sizeof(n1),"%d",num1);
        snprintf(n2,sizeof(n2),"%d",num2);
        snprintf(n3,sizeof(n3),"%d",num3);

        //Sending numbers to server
        if (send(cliSocket,n1,strlen(n1),0) <= 0){
            printf("Errore all'invio\n");
            closesocket(cliSocket);
            clearWinSock();
            return -1;
        }
    
        if (send(cliSocket,n2,strlen(n2),0) <= 0){
            printf("Errore all'invio\n");
            closesocket(cliSocket);
            clearWinSock();
            return -1;
        }
        
        if (send(cliSocket,n3,strlen(n3),0) <= 0){
            printf("Errore all'invio\n");
            closesocket(cliSocket);
            clearWinSock();
            return -1;
        }

        //Receiving result from server
        byteRcvd = recv(cliSocket, buffer, BUFFER_SIZE-1 ,0);
        if (byteRcvd <= 0){
            printf("Errore nella ricezione\n");
            closesocket(cliSocket);
            clearWinSock();
            return -1;
        }
        buffer[byteRcvd]='\0';
        printf("%s\n",buffer);

    } while (strcmp(buffer,"QUIT") != 0);
    
    
    system("pause");

    //Close connection
    closesocket(cliSocket);
    clearWinSock();

    return 0;
}

void clearWinSock(){
    #if defined WIN32
        WSACleanup();
    #endif
}