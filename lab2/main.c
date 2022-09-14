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

    int create_err = pthread_create(&thread, NULL, threadFunction, NULL);
    if (create_err){
        fprintf(stderr, "Error %d: %s\n", create_err, strerror(create_err));
    }

    int join_err = pthread_join(thread, NULL);
    if (join_err){
        fprintf(stderr, "Error %d: %s\n", join_err, strerror(join_err));
    }

    printLines("parent");

    return 0;
}
