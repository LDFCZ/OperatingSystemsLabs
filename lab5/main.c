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
#define EXECUTE 1  // 0 - при извлечении не исполнять обработчик

void cleanup_handler(void *param) {
    printf("\nThread canceled. Message from thread\n");
}

void *thread_print(void *param) {
    pthread_cleanup_push(cleanup_handler, NULL);
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
    pthread_cleanup_pop(EXECUTE); 
    pthread_exit(NULL);
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
    
    void* thread_output;
    int join_err = pthread_join(thread, &thread_output);
    if (join_err != CORRECT_CODE) {
        fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
    }

    if (thread_output == PTHREAD_CANCELED) {
        printf("Thread canceled\n");
    }

    return CORRECT_EXIT_CODE;
}

#undef CORRECT_CODE
#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef START_SYMBOL
#undef LAST_SYMBOL
#undef SLEEP_TIME
#undef EXECUTE
