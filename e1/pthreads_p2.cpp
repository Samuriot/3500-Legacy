//
// Created by Matthew Thayer on 1/13/22.
//

/*
 @file: pthreads_p2.cpp

 @author: Jomikael Ruiz, jruiz3@seattleu.edu

 @description: A program that demonstrates processes.

 @course: CPSC 3500
 @assignment: in-class activity [n]
 */

#include <pthread.h>
#include <iostream>

using namespace std;
int arr[10];


// This function shows the skeleton that pthread
// functions must adhere to.
// Copy this skeleton for any pthread function
// you need to define.
// In the copy, modify the name of the function
// and the function body as needed.
void *reverse(void *arg)
{
    // TODO: Add code that implements
    //       the thread's functionality
    cout << "The numbers in reverse: " << endl;
    for(int i = 9; i>=0; i--) {
        cout << arr[i] << endl;
    }
    return 0;
}

void *countNegatives(void *arg)
{
    // TODO: Add code that implements
    //       the thread's functionality
    int count = 0;
    for(int i = 0; i < 10; i++) {
        if(arr[i] < 0) {
            count++;
        }
    }
    cout << "Total negative numbers: " << count << endl << endl;
    return 0;
}

void *average(void *arg)
{
    // TODO: Add code that implements
    //       the thread's functionality
    int average = 0;
    for(int i = 0; i < 10; i++) {
        average += arr[i];
    }
    average = (average / 10);
    cout << "Average: " << average << endl << endl;
    return 0;
}


int main()
{
    // id is used to store a unique thread identifier,
    // returned by the call to create a new POSIX thread
    pthread_t id1;
    pthread_t id2;
    pthread_t id3;

    // rc is used to store the code returned by the
    // call to create a new POSIX thread. The value is
    // zero (0) if the call succeeds.
    int rc1;
    int rc2;
    int rc3;


    // TODO: Add code to perform any needed initialization
    //       or to process user input
    int input;
    for(int i = 0; i < 10; i++) {
        cout << "Please input a number: ";
        cin >> input;
        arr[i] = input;
    }



    // Create thread(s)
    // TODO: Modify according to assignment requirements
    rc1 = pthread_create(&id1, NULL, countNegatives, NULL);
    for(int count = 0; count < 100000; count++);
    rc2 = pthread_create(&id2, NULL, average, NULL);
    for(int count = 0; count < 100000; count++);
    rc3 = pthread_create(&id3, NULL, reverse, NULL);

    if(rc1){
        cout << "ERROR; return code from pthread_create() is " << rc1 << endl;
        return -1;
    }

    if(rc2){
        cout << "ERROR; return code from pthread_create() is " << rc2 << endl;
        return -1;
    }
    if(rc3){
        cout << "ERROR; return code from pthread_create() is " << rc3 << endl;
        return -1;
    }
    // NOTE: Using exit here will immediately end execution of all threads
    pthread_exit(0);
}


