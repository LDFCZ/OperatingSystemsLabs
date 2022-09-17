#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define START_SYMBOL 'A'
#define LAST_SYMBOL 'z'

void* thread_print(void *param){
    char symbol = START_SYMBOL;
    while (1){
        pthread_testcancel();
        printf("%c", symbol);
        if (++symbol == LAST_SYMBOL){
            symbol = START_SYMBOL;
        }
    }

    return NULL;
}

int main() {
    pthread_t thread;

    int create_err = pthread_create(&thread, NULL, thread_print, NULL);
    switch (create_err) {
        case EAGAIN:
        case EINVAL:
        case EPERM:
            fprintf(stderr, "Thread creating error %d: %s\n", create_err, strerror(create_err));
            return EXCEPTION_EXIT_CODE;
    }

    sleep(2);

    int cancel_err = pthread_cancel(thread);
    if (cancel_err == ESRCH){
        fprintf(stderr, "Thread canceling error %d: %s\n", cancel_err, strerror(cancel_err));
        return EXCEPTION_EXIT_CODE;
    }
    printf("\nThread canceled\n");

    int join_err = pthread_join(thread, NULL);
    switch (join_err) {
        case EDEADLK:
        case EINVAL:
        case ESRCH:
            fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
            return EXCEPTION_EXIT_CODE;
    }
    
    return CORRECT_EXIT_CODE;
}

#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef START_SYMBOL
#undef LAST_SYMBOL
