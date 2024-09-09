// Jomikael Ruiz
// wzip.cpp
// Apr. 8, 2024

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

const int BIT_SIZE = 1024;
const int STDOUT = 1;

int main(int argc, char ** argv) {
    // checking if arguments are empty
    if(argc == 1) {
        cout << "wzip file1 [file2 ...]" << endl;
        exit(1);
    }
    int readDescription;
    int readResult;
    int charCnt = 1;
    char readBuffer[BIT_SIZE];
    readDescription = open(argv[1], O_RDONLY);
    if(readDescription == -1) {
        cout << "wzip: cannot open file";
        exit(1);
    } else {
        do {
            readResult = read(readDescription, readBuffer, BIT_SIZE - 1);
            if(readResult == -1) {
                return 0;
            }
            for(int j = 0; j < readResult; j++) {
                while(readBuffer[j] == readBuffer[j+1]) {
                    j++;
                    charCnt++;
                }
                write(STDOUT, (char*)&charCnt, sizeof(int));
                write(STDOUT, (char*)&readBuffer[j], sizeof(char));
                charCnt = 1;
            }
        } while(readResult > 0);
    }
    close(readDescription);
    return 0;
}
