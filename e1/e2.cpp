//
// Created by Jomikael Ruiz on 4/3/24.
//

#include <iostream>
#include <unistd.h>

int main() {
    char *args[] = {"/bin/echo", "hello", NULL};
    for(int i = 0; i < 3; i++) {
        execvp(args[0], args);
    }
    return 0;
}
