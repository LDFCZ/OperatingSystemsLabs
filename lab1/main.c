#include <stdio.h>
#include <pthread.h>
#include <string.h>

void printLines(char* str) {
    for (int i = 0; i < 10; i++) {
        printf("%s %d\n", str, i);
    }
}

void* threadFunction(void *param){

    printLines("child");

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thread;
    
    int err = pthread_create(&thread, NULL, threadFunction, NULL);
    if (err){
        fprintf(stderr, "Error %d: %s\n", err, strerror(err));
    }

    printLines("parent");

    pthread_exit(NULL);
}
