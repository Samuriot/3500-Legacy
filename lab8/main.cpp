// Jomikael Ruiz
// May 21st, 2024
// main.cpp
// Purpose: creating a client side app that connects to a server

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

#define BUFFERMAX 1000000

int main(int argc, char** argv) {
    struct sockaddr_in newSocket;
    char * hostNum = nullptr;
    int portNum;
    int socketVar;
    int index;
    int result;
    char buffer[BUFFERMAX + 1];
    // create a socket
    if (argc == 3) {
        hostNum = argv[1];
        portNum = atoi(argv[2]);
    } else {
        cout << "usage: client-tcp <hostnum> <portnum>" << endl;
        exit(-1);
    }

    // specify the server address and port
    newSocket.sin_family = AF_INET;
    if ((newSocket.sin_addr.s_addr = inet_addr(hostNum)) == INADDR_NONE)
    {
        cout << "Invalid dotted decimal address" << endl;

        exit(-1);
    }
    newSocket.sin_port = htons(portNum);
    // connect to the server
    socketVar = socket(PF_INET, SOCK_STREAM, 0);

    if ((result = connect (socketVar, (struct sockaddr *) &newSocket, sizeof(newSocket))))
    {
        perror("Connect failed\n");
        close (socketVar);
        exit(-1);
    };

    cout << "Connected to server at IP address = " << hostNum;
    cout << " and Port = " << portNum << endl;

    // receive data from the server and print
    char * charPtr = buffer;
    int count = BUFFERMAX;

    if ( (index = recv (socketVar, charPtr, count, 0)) > 0 )
    {
        charPtr += index;
        count = BUFFERMAX - (charPtr - buffer);
    }


    if (index != -1)
    {
        *charPtr = '\0';
        cout << "Response was: " << buffer << endl;
    }
    else
    {
        perror("recv failed");
        exit(-1);
    }

    // send data to the server
    string message = "Hello, server!";
    int total_characters = message.length();
    result = send (socketVar, message.c_str(), total_characters, 0);

    if (result == -1) {
        perror("Send failed");
        exit(-1);
    }
    // close the socket
    close(socketVar);

    return 0;
}