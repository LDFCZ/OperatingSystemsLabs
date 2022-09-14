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

    int create_err = pthread_create(&thread, NULL, threadFunction, NULL);
    if (create_err){
        fprintf(stderr, "Error %d: %s\n", create_err, strerror(create_err));
    }

    int join_err = pthread_join(thread, NULL);
    if (join_err){
        fprintf(stderr, "Error %d: %s\n", join_err, strerror(join_err));
    }

    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
    }
    
    return 0;
}