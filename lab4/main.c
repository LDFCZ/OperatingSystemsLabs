#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* thread_print(void *param){
    char symbol = 'A';
    while (1){
        pthread_testcancel();
        printf("%c", symbol);
        if (++symbol == 'z'){
            symbol = 'A';
        }
    }

    return NULL;
}

int main() {
    pthread_t thread;

    int create_err = pthread_create(&thread, NULL, thread_print, NULL);
    if (create_err){
        fprintf(stderr, "Error %d: %s\n", create_err, strerror(create_err));
    }

    sleep(2);

    int cancel_err = pthread_cancel(thread);
    if (cancel_err){
        fprintf(stderr, "Error %d: %s\n", cancel_err, strerror(cancel_err));
    }
    printf("\nThread canceled\n");

    int join_err = pthread_join(thread, NULL);
    if (join_err){
        fprintf(stderr, "Error %d: %s\n", join_err, strerror(join_err));
    }
    
    return 0;
}