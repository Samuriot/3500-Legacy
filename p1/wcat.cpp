// Jomikael Ruiz
// wcat.cpp
// April 8, 2024

#include <iostream>
#include <fstream>
using namespace std;

const int NO_ARGS = 1;

// function to print file
int printFile(string fileName);

int main(int argc, char ** argv) {
    // if no arguments are passed, exit with code one
    if(argc == NO_ARGS) {
        cout << "wcat file [file ...]" << endl;
        exit(1);
    }
    int result;
    // loop to iterate through all command arguments
    for(int i = NO_ARGS; i < argc; i++) {
        result = printFile(argv[i]);
        if(result == -1) {
            exit(1);
        }
    }
    return 0;
}

int printFile(string fileName) {
    fstream inFile;
    string read;
    // file processing using fstream
    inFile.open(fileName);
    // error handling in the case of an incorrect file
    if(inFile.fail()) {
        cout << "wcat: cannot open file";
        return -1;
    }
    // reading each line of text and printing to the screen
    while(getline(inFile, read)) {
        cout << read << endl;
    }
    // closing file before closing program
    inFile.close();
    return 0;
}
