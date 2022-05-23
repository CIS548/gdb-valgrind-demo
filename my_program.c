#include <stdio.h>

void printHello() {
    printf("Hello World\n");
}

void printString(char* string) {
    printf("argument: %s\n", string);
    printf("first char of argument: %c\n", string[0]);
}

int main(int argc, char** argv) {
    printf("argc = %d\n", argc);
    printf("argv[0] = %s\n", argv[0]);
    printHello();
    if (argc == 2) {
        printString(argv[1]);
    }
    
}

