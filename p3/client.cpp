// Jomikael Ruiz
// client.cpp
// 6/3/24
// Purpose: developing a client application which connects to a server to be used in tandem with
// server.cpp. Requests an arithmetic function and sends that data to the server

// include statements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char *argv[]) {
   // variables
   struct sockaddr_in socketOne;
   int socketVar;
   int portNum;
   char *hostNum;

   int connectResult = 0, sendResult = 0, readResult = 0;
   // Get the port number and dotted decimal address of server
   // from the command-line arguments
   if (argc == 3) {
      hostNum = argv[1];
      portNum = atoi(argv[2]);
   } else {
      cout << "usage: client-tcp <hostnum> <portnum>" << endl;
      exit(-1);
   }

   // Initialize data structure
   socketOne.sin_family = AF_INET;
   if ((socketOne.sin_addr.s_addr = inet_addr(hostNum)) == INADDR_NONE) {
      cout << "Invalid dotted decimal address" << endl;
      exit(-1);
   }

   // initializing port number and making the socket
   socketOne.sin_port = htons(portNum);
   socketVar = socket(PF_INET, SOCK_STREAM, 0);

   // connecting to the server through the socket
   if ((connectResult = connect(socketVar, (struct sockaddr *) &socketOne, sizeof(socketOne)))) {
      cout << "connect failed" << endl;
      close(socketVar);
      exit(-1);
   }

   cout << "Connected to server at IP address = " << hostNum << " and Port = " << portNum << endl;
   // Run indefinitely,
   while(1) {
      // Read a message from the standard input
      cout << "Input your request: ";
      string msg;
      getline (cin, msg);

      // Send the message
      int total_characters = msg.length();
      sendResult = send(socketVar, msg.c_str(), total_characters, 0);
      if(sendResult == -1) {
         cout << "send failed" << endl;
         close(socketVar);
         exit(-1);
      }
      
      // buffer for messages
      const int BUFFER_SIZE = 1024;
      char buffer[BUFFER_SIZE] = {0}; // Create a buffer of size 1024 Bytes and default it to 0.

      // receives message from the client with error handling
      readResult = recv(socketVar, buffer, BUFFER_SIZE, 0);
      if(readResult <= 0) {
         cout << "read failed" << endl;
         close(socketVar);
         exit(-1);
      }

      cout << "Server replied with: " << buffer << endl;
   }

   // close socket
   close(socketVar);
   return 0;
}
