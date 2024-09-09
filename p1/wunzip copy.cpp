// Jomikael Ruiz
// wunzip.cpp
// April 8th, 2024

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

const int NO_ARGS = 1;

// function dedicated to handling a single file and decrypting its contents
int fileHandle(string fileName);
// function that takes in a string of characters and decrypts its contents
void handleEncrypt(string read);

int main(int argc, char ** argv) {
    // if no command line arguments are passed, exit with code 1
    if(argc == NO_ARGS) {
        cout << "wunzip file1 [file2 ...]" << endl;
        exit(1);
    }
    int result;
    // loop to iterate through argv[]
    for(int i = NO_ARGS; i < argc; i++) {
        result = fileHandle(argv[i]);
        if(result == 1) {
            exit(1);
        }
    }
    return 0;
}

void handleEncrypt(string read) {
    char character;
    int charCnt = 0;
    stringstream s(read);
    // reading the encrypted file and decoding it, and printing to the screen
    while(s.read((char*)&charCnt, sizeof(int)) && s.get(character)) {
        for (int i = 0; i < charCnt; i++) {
            cout << character;
        }
    }
}

int fileHandle(string fileName) {
    fstream inFile;
    string read;
    // file handling through fstream
    inFile.open(fileName);
    if(inFile.fail()) {
        cout << "wunzip: cannot open file";
        return 1;
    }
    // handling the file using string stream and getline
    while(getline(inFile, read)) {
        // calls encrypt file
        handleEncrypt(read);
        cout << endl;
    }
    // close file after use
    inFile.close();
    return 0;
}