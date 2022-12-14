#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CORRECT_CODE 0
#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define START_SYMBOL 'A'
#define LAST_SYMBOL 'z'
#define SLEEP_TIME 2

void* thread_print(void *param) {
    char symbol = START_SYMBOL;
    while (1) {
        pthread_testcancel();
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        printf("%c", symbol);
        if (++symbol == LAST_SYMBOL){
            symbol = START_SYMBOL;
        }
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    }

    return NULL;
}

int main() {
    pthread_t thread;

    int create_err = pthread_create(&thread, NULL, thread_print, NULL);
    if (create_err != CORRECT_CODE) {
        fprintf(stderr, "Thread creating error %d: %s\n", create_err, strerror(create_err));
        return EXCEPTION_EXIT_CODE;
    }

    sleep(SLEEP_TIME);

    int cancel_err = pthread_cancel(thread);
    if (cancel_err != CORRECT_CODE) {
        fprintf(stderr, "Thread canceling error %d: %s\n", cancel_err, strerror(cancel_err));
    }
    printf("\nCancel thread\n");

    void *thread_return_value;
    int join_err = pthread_join(thread, &thread_return_value);
    if (join_err != CORRECT_CODE) {
        fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
    }

    if (thread_return_value == PTHREAD_CANCELED) {
	    printf("Thread was canceled!\n");
    }
    
    return CORRECT_EXIT_CODE;
}

#undef CORRECT_CODE
#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef START_SYMBOL
#undef LAST_SYMBOL
#undef SLEEP_TIME
