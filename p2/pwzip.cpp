// Jomikael Ruiz
// pwzip.cpp
// May 12th, 2024
// Purpose: Exploring Multithreading capabilities by expanding upon wzip.cpp

// include statements
#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <deque>
#include <sstream>
#include <sys/sysinfo.h> 
using namespace std;

// constants
const int NO_ARGS = 1;

// Args struct for dividing up the work amongst
struct Args {
    long start;
    long end;
    int threadNum;
};

struct Letters {
    vector<char> letters;
    vector<int> nums;
    long count = 0;
    bool state = false;
};

// global variables for pthreads
char * file_contents = nullptr;
deque<Letters> buffer;
pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;

// producer function that reads data from global char * and takes the letterCnt and letter char
// precondition: arg contains a struct Args that has start, end, and threadNum >= 0
// postcondition: values from data read are palced in a global buffer for consumer
void * readPhrase(void * arg);

// consumer function that takes data from a buffer and writes the encryption to stdout
// precondition: arg contains a struct Args that has start, end, and threadNum >= 0
// postcondition: values from the global buffer are written to stdout
void * encryptPhrase(void * arg);

// args generation function to denote the start and ending index + thread numbers for threads
// precondition: s, e, id >= 0
// postcondition: Args struct is returned with these values assigned to start, end, and threadNum
Args generateArgs(long s, long e, int id);

int main(int argc, char ** argv) {
    // handling if no arguments are passed in
    if(argc == NO_ARGS) {
        cout << "wzip file1 [file2 ...]" << endl;
        exit(1);
    }

     // setting up pthreads and other variables
    int NUM_THREADS = get_nprocs();
    int NUM_PRODUCERS = NUM_THREADS - 1;
    int NUM_CONSUMERS = 1;
    pthread_t * id = new pthread_t[NUM_THREADS];
    int * rc = new int[NUM_THREADS];
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
        file_contents = nullptr;
        struct stat sb;
        Letters emptyLetters;

        // mapping file to memory
        *fileDescription = open(argv[i], O_RDONLY);
        if(*fileDescription == -1) {
            cout << "wzip: cannot open file";
            delete fileDescription;
            delete file_contents;
            exit(1);
        }
        fstat(*fileDescription, &sb);
        void * mmap_return = (char*)mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, *fileDescription, 0);
        file_contents = (char*)mmap_return;
        
        // dividing work for producers
        elementsPerThread = sb.st_size / NUM_PRODUCERS;
        remainder = sb.st_size % NUM_PRODUCERS;
        for(int i = 0; i < NUM_PRODUCERS; i++) {
            long start = i * elementsPerThread + min(i,remainder);
            long end = start + elementsPerThread - (i < remainder ? 0:1);
            args.push_back(generateArgs(start, end, i));
            // to prevent an out of bounds error
            buffer.push_back(emptyLetters);
        }

        // producer threads
        for(int i = 0; i < NUM_PRODUCERS; i++) {
            void * argToPass = &args[i];
            rc[i] = pthread_create(&id[i], nullptr, readPhrase, argToPass);
        }
        // consumer threads
        elementsPerThread = NUM_PRODUCERS;
        args.push_back(generateArgs(0, NUM_PRODUCERS, NUM_THREADS));
        void * argToPass = &args[NUM_THREADS-1];
        rc[NUM_THREADS-1] = pthread_create(&id[NUM_THREADS-1], nullptr, encryptPhrase, argToPass);
        // checks to see if any failures occur
        for(int i = 0; i < NUM_THREADS; i++) {
            if(rc[i]) {
                cout << "Error while creating thread: " << args[i].threadNum;
                exit(-1);
            }
        }
        // joins all threads and waits for everything to finish
        for(int i  = 0; i < NUM_THREADS; i++) {
            pthread_join(id[i], NULL);
        }
        // unmaps and closes file
        munmap(file_contents, sb.st_size);
        close(*fileDescription);
        delete fileDescription;
        delete [] id;
        delete [] rc;
    }
    return 0;
}

// producer function
void * readPhrase(void * arg) {
    // basic vars need to read and store data
    Args argument = *(Args *) arg;
    Letters l;
    int charCnt = 0;
    char currChar;
    // loop to iterate through whole phrase
    for(off_t j = argument.start; j <= argument.end; j++) {
        currChar = file_contents[j];
        // checks to see where there are multiple chars
        while(file_contents[j] == currChar && j <= argument.end) {
            j++;
            charCnt++;
        }
        j--;
        l.letters.push_back(currChar);
        l.nums.push_back(charCnt);
        l.count++;
        charCnt = 0;
    }
    // statement flag for consumer to see if node is fully processed
    l.state = true;
    // locking mutex to prevent race conditions
    pthread_mutex_lock(&data_lock);
    buffer.at(argument.threadNum) = l;
    pthread_mutex_unlock(&data_lock);   
    pthread_exit(0);
}

// consumer function
void * encryptPhrase(void * arg) {
    // casting args to see the assignment of the thread
    Args argument = *(Args *) arg;
    for(int i = argument.start; i < argument.end; i++) {
        Letters encrypted = buffer.at(i);
        // spin lock to wait if node is not ready
        while(!encrypted.state);
        for(int j  = 0; j < encrypted.count; j++) {
            // takes content of the node and prints it out to stdout
            cout.write((char*)&encrypted.nums[j], sizeof(int));
            cout.write((char*)&encrypted.letters[j], sizeof(char));
        }
    }
    pthread_exit(0);
}

// args generation function
Args generateArgs(long s, long e, int id) {
    Args a;
    a.start = s;
    a.end = e;
    a.threadNum = id;
    return a;
}