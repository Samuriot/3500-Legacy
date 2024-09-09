// Jomikael Ruiz
// June 3rd, 2024
// server.cpp
// Purpose: developing a server application to be used in tandem with
// client.cpp. Receives an arithmetic operation from client and sends back
// the result of the operation

// include statements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>

using namespace std;

// constants
const string ADD = "+";
const string MINUS = "-";
const string DIVIDE = "/";
const string MULTIPLY = "*";

// functions

// calculate function that takes a string input and evaluates the expression
// precondition: input is a valid string with a valid math operation
// postcondition: returns a numerical value from the expression
int calculate(string input);


int main(int argc, char *argv[]) {
   // variables
   struct sockaddr_in socketOne, newSocket;
   int server, childSocket;
   int portNum;
   int sendResult, listenResult, receiveResult;

   // Get the port number from the command-line arguments
   if (argc == 2) {
      portNum = atoi(argv[1]);
   } else {
      cout << "usage: server-tcp <portnum>" << endl;
      exit(-1);
   }

   // Populate the data structure
   // ip address and port number
   socketOne.sin_family = AF_INET;
   socketOne.sin_addr.s_addr = INADDR_ANY;
   socketOne.sin_port = htons(portNum);

   // creates socket and binds to a port
   server = socket(PF_INET, SOCK_STREAM, 0);
   bind(server, (struct sockaddr *) &socketOne, sizeof(socketOne));

   cout << "Server started" << endl;
   cout << "Listening on port " << portNum << endl;
   cout << "looking for clients" << endl;
   // listenResult = listen(server, 1);
   // if(listenResult == -1) {
   //    cout << "listen failed" << endl;
   //    close(server);
   //    exit(-1);
   // }

         // listens for clients to create a queue of clients
      listenResult = listen(server, 5);
      if(listenResult == -1) {
         cout << "listen failed" << endl;
         close(server);
         exit(-1);
      }

   // makes sure server is always running
   while(1) { 
      // creates a socket for the client under childSocket
      socklen_t addressLength = sizeof(newSocket);
      childSocket = accept(server, (struct sockaddr*) &newSocket, &addressLength);
      
      // while loop for while the client is still connected
      while(childSocket != -1) {
         // char *buffer = bufferInput;
         // int cnt = MAXBUF;

         // buffer for messages
         const int BUFFER_SIZE = 1024;
         char buffer[BUFFER_SIZE] = {0}; // Create a buffer of size 1024 Bytes and default it to 0.

         // receives message from the client with error handling
         receiveResult = recv(childSocket, buffer, BUFFER_SIZE, 0);
         if(receiveResult <= 0) {
            close(childSocket);
            break;
         }
         // if((counter = recv(childSocket, buffer, cnt, 0)) > 0 ) {
         //    cout << "received a message" << endl;
         //    buffer += counter;
         //    cnt = MAXBUF - (buffer - bufferInput);
         // }
         // *buffer = '\0';

         // prints the data received from the buffer
         cout << "Socket " << childSocket << " sent message: " << buffer << endl;

         // handles the calculations from the message
         string convertedBuffer(buffer);
         int finishedOp = calculate(convertedBuffer);
         string clientResponse = to_string(finishedOp);
         const char *message_arg = clientResponse.c_str();

         // returns the calculated response back to client
         cout << "sending reply: " << clientResponse << endl;
         sendResult = send(childSocket, message_arg, clientResponse.length(), 0);
         if(sendResult == -1) {
            cout << "send failed" << endl;
            break;
         }
      }
      // close the socket before reactivating another socket
      close(childSocket);
   }
   cout << "exiting program" << endl;
   close(server);

   return 0;
}

int calculate(string input) {
   // variables
   int size = input.length();
   int index = 0;
   int numOne, numTwo, finishedOp = -1;
   string unparseNumOne = "", unparseNumTwo = "", operand = "";

   // iterate through whole string
   for(int i = 0; i < size; i++) {
      // gets indices of first number
      while(isdigit(input[index])) {
         index++;
      }
      // add first number to unparseNumOne
      while(i < index) {
         unparseNumOne += input[i];
         i++;
      }
      numOne = stoi(unparseNumOne);
      // if there is a space between operand and numbers, skip the space
      while(input[i] == ' ') {
         i++;
      }
      // now that index is at operand, add it to the operand string
      operand = input[i];
      i++;
      while(input[i] == ' ') {
         i++;
      }
      // get index of the second number
      index = i;
      while(isdigit(input[index])) {
         index++;
      }
      // add second number to unparseNumTwo
      while(i < index) {
         unparseNumTwo += input[i];
      i++;
      }
      numTwo = stoi(unparseNumTwo);
   }

   if(operand == ADD) {
      finishedOp = numOne + numTwo;
   } else if(operand == MINUS) {
      finishedOp = numOne - numTwo;
   } else if(operand == DIVIDE) {
      finishedOp = numOne / numTwo;
   } else if(operand == MULTIPLY) {
      finishedOp = numOne * numTwo;
   }
   return finishedOp;
}