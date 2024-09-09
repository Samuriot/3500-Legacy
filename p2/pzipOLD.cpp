// Jomikael Ruiz
// pzip.cpp
// April 19th, 2024
// Purpose: Exploring Multithreading capabilities by expanding upon wzip.cpp

#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <deque>
#include <sstream>
using namespace std;

// constants
const int NO_ARGS = 1;
const int NUM_THREADS = 4;
const int NUM_PRODUCERS = 2;
const int NUM_CONSUMERS = 2;

// structures
struct Args {
    long start;
    long end;
    int threadNum;
};

struct Letters {
    vector<char> letters;
    vector<int> nums;
    stringstream * ss = nullptr;
    int count = 0;
};

// global variables for pthreads
char * file_contents = nullptr;
int next_chunk = 1;
deque<Letters> buffer;
pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t stdout_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notPopulated = PTHREAD_COND_INITIALIZER;

// function dedicated to processing a file and encrypting its contents
void * readPhrase(void * arg);
// function that takes in a string of characters and encrypts its contents
void * encryptPhrase(void * arg);
// function dedicated to creating Args for pthreads
Args generateArgs(long s, long e, int id);
// function dedicated to creating Letters for consumer threads
Letters generateLetters(char l, int var);

int main(int argc, char ** argv) {
    // handling if no arguments are passed in
    if(argc == NO_ARGS) {
        cout << "wzip file1 [file2 ...]" << endl;
        exit(1);
    }

     // setting up pthreads and other variables, 2 for producer, 2 for consumer
    pthread_t id[NUM_THREADS];
    int rc[NUM_THREADS];
    long elementsPerThread;
    int remainder;
    int * fileDescription = nullptr;
    string print;
    vector<Args> args;
    vector<Letters> letters;

    // loops through argv
    for(int i = NO_ARGS; i < argc; i++) {
        // initializing pointers
        fileDescription = new int;
        file_contents = new char;
        struct stat sb;
        Letters emptyLetters;

        // mapping file to memory
        *fileDescription = open(argv[i], O_RDONLY);
        // error handling in case file open fails
        if(*fileDescription == -1) {
            cout << "wzip: cannot open file";
            exit(1);
        }

        fstat(*fileDescription, &sb);
        void * mmap_return = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, *fileDescription, 0);
        file_contents = (char*)mmap_return;
        
        // dividing work for producers
        elementsPerThread = sb.st_size / NUM_PRODUCERS;
        remainder = sb.st_size % NUM_PRODUCERS;
        for(int i = 0; i < NUM_PRODUCERS; i++) {
            long start = i * elementsPerThread + min(i,remainder);
            long end = start + elementsPerThread - (i < remainder ? 0:1);
            cout << "starting at: " << start << " and ending at: " << end;
            args.push_back(generateArgs(start, end, i));
            // to prevent an out of bounds error
            buffer.push_back(emptyLetters);
        }

        // producer threads
        for(int i = 0; i < NUM_PRODUCERS; i++) {
            // be aware of race conditions
            void * argToPass = &args[i];
            rc[i] = pthread_create(&id[i], nullptr, readPhrase, argToPass);
        }
        for(int i = 0; i < NUM_PRODUCERS; i++) {
            if(rc[i]) {
                cout << "Error while creating thread: " << args[i].threadNum;
                exit(-1);
            }
        }
        
        /**
         * split up consumer work reading the file and place it into a buffer
         * buffer is accessed by consumer and begins encryption
        */
        // consumer thread
        // dividing work for producers
        elementsPerThread = NUM_PRODUCERS / NUM_CONSUMERS;
        remainder = NUM_PRODUCERS % NUM_CONSUMERS;
        for(int i = 0; i < NUM_CONSUMERS; i++) {
            long start = i * elementsPerThread + min(i,remainder);
            long end = start + elementsPerThread - (i < remainder ? 0:1);
            args.push_back(generateArgs(start, end, i+NUM_CONSUMERS));
            // to prevent an out of bounds error
        }

        for(int i = NUM_THREADS - NUM_CONSUMERS; i < NUM_THREADS; i++) {
            void * argToPass = &args[i];
            rc[i] = pthread_create(&id[i], nullptr, encryptPhrase, argToPass);
            if(rc[i]) {
                cout << "Error while creating thread: " << args[i].threadNum;
                exit(-1);
            }
        }

        for(int i  = 0; i < NUM_THREADS; i++) {
            pthread_join(id[i], NULL);
        }

        for(size_t i = 0; i < buffer.size(); i++) {
            // put into its respective data type
            cout << buffer.at(i).ss->rdbuf();
            delete buffer.at(i).ss;
        }
        munmap(file_contents, sb.st_size);
        close(*fileDescription);
    }
    delete fileDescription;
    return 0;
}

// producer function
void * readPhrase(void * arg) {
    // counter to keep track of number of characters
    cout << "entering thread" << endl;
    Args argument = *(Args *) arg;
    Letters l;
    int charCnt = 0;
    char currChar;
    // loop to iterate through whole phrase
    for(off_t j = argument.start; j <= argument.end; j++) {
        currChar = file_contents[j];
        while(file_contents[j] == currChar && j <= argument.end) {
            j++;
            charCnt++;
        }
        j--;
        cout << charCnt << file_contents[j];
        l.letters.push_back(currChar);
        l.nums.push_back(charCnt);
        l.count++;
        charCnt = 0;

    }
    buffer.at(argument.threadNum) = l;
    next_chunk++;
    pthread_exit(0);
}

// consumer function
void * encryptPhrase(void * arg) {
    // if buffer is empty, wait for buffer to not be empty
    // add flags to see if array is written to already
    // use conditions and checks
    cout << "entering consumer thread" << endl;
    Args argument = *(Args *) arg;
    for(int i = argument.start; i <= argument.end; i++) {
        if(i >= next_chunk) {
            pthread_cond_wait(&notPopulated, &data_lock);
        }
        Letters encrypted = buffer.at(i);
        encrypted.ss = new stringstream;
        for(int j  = 0; j < encrypted.count; j++) {
            // cout << encrypted.nums[j] << encrypted.letters[j];
            encrypted.ss->write((char*)&encrypted.nums[j], sizeof(int));
            encrypted.ss->write((char*)&encrypted.letters[j], sizeof(char));
        }
        buffer.at(i) = encrypted;
    }
    pthread_exit(0);
}

Args generateArgs(long s, long e, int id) {
    Args a;
    a.start = s;
    a.end = e;
    a.threadNum = id;
    return a;
}