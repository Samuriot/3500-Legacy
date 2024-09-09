// Jomikael Ruiz
// wgrep.cpp
// April 8, 2024

#include <iostream>
#include <fstream>
using namespace std;

const int NO_ARGS = 1;
const int ONE_ARG = 2;

int processFile(string fileName, string word);
void processWord(string readWord, string word);

int main(int argc, char ** argv) {
    if(argc == NO_ARGS) {
        cout << "wcat file [file ...]" << endl;
        exit(1);
    }
    // gets the word to be searched for from command line
    string word = argv[NO_ARGS];
    string input;
    int result;
    // loop and if statement to check if only the word is passed in without files
    if(argc == ONE_ARG) {
        while(true) {
            getline(cin, input);
            processWord(input, word);
        }
    }
    for(int i = ONE_ARG; i < argc; i++) {
        result = processFile(argv[i], word);
        if(result == 1) {
            exit(1);
        }
    }
    return 0;
}

int processFile(string fileName, string word) {
    string read;
    fstream inFile;
    // file processing
    inFile.open(fileName);
    if(inFile.fail()) {
        cout << "wcat: cannot open file";
        return 1;
    }
    while(getline(inFile, read)) {
        // calls processWord to search if line has the word to be searched
        processWord(read, word);
    }
    inFile.close();
    return 0;
}

void processWord(string readWord, string word) {
    // ptr to keep track of a word
    int wordPtr;
    for(int i = 0; i < readWord.length(); i++) {
        wordPtr = 0;
        // loop to see if char/string is in the line
        while(readWord[i] == word[wordPtr] && i < readWord.length()) {
            i++;
            wordPtr++;
        }
        // if string is in the line, print the line and break out of loop
        if(wordPtr == word.length()) {
            cout << readWord << endl;
            break;
        }
    }
}
