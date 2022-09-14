#include <stdio.h>
#include <pthread.h>
#include <string.h>

void* threadFunction(void *param){

    for (int i = 10; i < 20; i++) {
        printf("%d\n", i);
    }

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thread;
    
    int err = pthread_create(&thread, NULL, threadFunction, NULL);
    if (err){
        fprintf(stderr, "Error %d: %s\n", err, strerror(err));
    }

    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
    }

    pthread_exit(NULL);
}