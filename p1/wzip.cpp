// Jomikael Ruiz
// wzip.cpp
// April 8th, 2024

#include <iostream>
#include <fstream>
using namespace std;

const int NO_ARGS = 1;

// function dedicated to processing a file and encrypting its contents
int processFile(string fileName);
// function that takes in a string of characters and encrypts its contents
void encryptPhrase(string& read);

int main(int argc, char ** argv) {
    // handling if no arguments are passed in
    if(argc == NO_ARGS) {
        cout << "wzip file1 [file2 ...]" << endl;
        exit(1);
    }
    int result;
    // loops through argv
    for(int i = NO_ARGS; i < argc; i++) {
        result = processFile(argv[i]);
        // if error is sent from processFile, exit with code 1
        if(result == NO_ARGS) {
            exit(1);
        }
    }
}

void encryptPhrase(string& read) {
    // counter to keep track of number of characters
    int charCnt = 1;
    // loop to iterate through whole phrase
    for(int j = 0; j < read.length(); j++) {
        // checks to see where there are multiple chars
        while(read[j] == read[j+1]) {
            j++;
            charCnt++;
        }
        // writes the output of encryption to stdout
        cout.write((char*)&charCnt, sizeof(int));
        cout.write((char*)&read[j], sizeof(char));
        charCnt = 1;
    }
}

int processFile(string fileName) {
    fstream inFile;
    string read;
    // file handling using fstream
    inFile.open(fileName);
    if(inFile.fail()) {
        cout << "wzip: cannot open file";
        return 1;
    }
    // loop to read through file
    while(getline(inFile, read)) {
        // calls encryptPhrase to encrypt the line of text from file
        encryptPhrase(read);
        cout << '\n';
    }
    inFile.close();
    return 0;
}