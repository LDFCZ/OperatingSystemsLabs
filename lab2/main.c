#include <stdio.h>
#include <pthread.h>
#include <string.h>

void* threadFunction(void *param){
    printf("6\n");
    printf("7\n");
    printf("8\n");
    printf("9\n");
    printf("10\n");
    
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thread;

    int create_err = pthread_create(&thread, NULL, threadFunction, NULL);
    if (create_err){
        printf("Error %d: %s\n", create_err, strerror(create_err));
    }

    int join_err = pthread_join(thread, NULL);
    if (join_err){
        printf("Error %d: %s\n", join_err, strerror(join_err));
    }

    printf("1\n");
    printf("2\n");
    printf("3\n");
    printf("4\n");
    printf("5\n");

    return 0;
}