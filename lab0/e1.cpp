// Jomikael Ruiz
// e1.cpp
// Mar 27, 2024

#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    pid_t id = fork();
    if(id == -1) {
        cout << "There has been an error in creating the process";
        return 0;
    } else if(id == 0) {
        char* args[] = {"echo", "Hello from child process. Child Process created and executed!", NULL};
        execvp(args[0], args);
    } else {
        cout << "Parent process has been created and executed!" << endl;
        int status;
        waitpid(id, &status, 0);
    }
    return 0;
}
