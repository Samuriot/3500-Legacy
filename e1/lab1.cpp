//
// Created by Matthew Thayer on 3/30/23.
//

// Jomikael Ruiz
// Apr. 6, 2024
// lab1.cpp
// Purpose: working with execvp and processes

#include <iostream>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <fstream>
using namespace std;

void signal_handler();

int main(){

    // After generating byte code for Print.cpp
    // Enter file path to byte code
    string filename;
    cout << "Path to compiled Print.cpp file called Print: ";
    getline(cin, filename);

    // Adding Print bye code to parents path variable
    // so child can recognize file when trying to execvp
    ifstream inFile;
    inFile.open(filename);


    // Check to make sure child exists
    if(inFile){
        pid_t id = fork();
        if(id == -1){
            cout << "Error creating child!" << endl;
        }
        else if(id == 0){
            /**
             * TODO:
             *      1. Call "../ice/Print" using execvp
             *      2. Pass in 2 command line arguments to Print
             *          First is number of times message will print
             *          Second is the message to print
             */
            string numPrint = "6";
            string printedMessage = "Hello";
            char * filePath = new char[filename.length() + 1];
            char * numPrintConverted = new char[numPrint.length() + 1];
            char * printedMessageConverted = new char[printedMessage.length() + 1];
            copy(filename.begin(), filename.end(), filePath);
            copy(numPrint.begin(), numPrint.end(), numPrintConverted);
            copy(printedMessage.begin(), printedMessage.end(), printedMessageConverted);
            char * argument_list[] = {filePath, numPrintConverted, printedMessageConverted, NULL};
            execvp(filePath, argument_list);
        }
        else{
            /**
             * TODO:
             *      1. Set up signal handler for parent
             *          Signal to handle will be SIGUSR1
             *          Handler should:
             *              print signal received and child just finished message
             *              exit with code 0;
             */

            signal(SIGUSR1, reinterpret_cast<void (*)(int)>(signal_handler));
            // Have parent sleep until signal is received
            sleep(10);

        }
    }
    else{
        cout << "Could not open file!";
        exit(-1);
    }

    // If either process get to this line then signal was not received
    // exiting with code -1
    exit(-1);

}

void signal_handler() {
    cout << "Signal Received and child just finished." << endl;
    exit(0);
}