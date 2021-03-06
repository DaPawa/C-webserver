#include "common.h"

SOCKET webserverStartUp(){
        WSADATA wsa;
    SOCKET listenSocket;

    //Starts up the winsock in version 2.2 and checks for any errors
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        printf("Error: %d\n", WSAGetLastError());
        exit(-1);
    }
    printf("Initialised\n");

    //Creates the address info structures to be used in getaddrinfo();
    struct addrinfo *result = NULL, hints;

    //Zeros out the memory in hints and defines it's values
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, DEFAULT_PORT, &hints, &result) != 0){
        printf("getaddrinfo failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(-1);
    }

    //Creates the socket to listen with and checks for any errors
    if((listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET){
        printf("Couldn't create socket: %d\n", WSAGetLastError());
        exit(-1);
    }
    printf("Socket created\n");

    if((bind(listenSocket, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR){
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        exit(-1);
    }

    freeaddrinfo(result);
    printf("Socket bound\n");

    if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(-1);
    }

    printf("Listening on socket\n");
    return listenSocket;
}
