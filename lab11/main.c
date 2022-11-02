#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define EXCEPTION_CODE 1
#define CORRECT_CODE 0
#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define SRT_COUNT 5

pthread_mutex_t mutex1, mutex2, startMutex;

void print_error(int return_code, char *additional_message) {
    fprintf(stderr, "%s %d: %s\n", additional_message, return_code, strerror(return_code));
}

void *print_strings(void *param) {
    int lock_code, unlock_code;
    lock_code = pthread_mutex_lock(&startMutex);
    if (lock_code != CORRECT_CODE) {
        print_error(lock_code, "Mutex lock error");
        pthread_exit(NULL);
    }

    for (int i = 0; i < SRT_COUNT; ++i) {
        lock_code = pthread_mutex_lock(&mutex2);
        if (lock_code != CORRECT_CODE) {
            print_error(lock_code, "Mutex lock error");
            pthread_exit(NULL);
        }

        printf("Thread - %d\n", i);

        unlock_code = pthread_mutex_unlock(&mutex1);
        if (unlock_code != CORRECT_CODE) {
            print_error(unlock_code, "Mutex unlock error");
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

int initialize_mutexes() {
    int init_code;
    init_code = pthread_mutex_init(&startMutex, NULL);
    if (init_code != CORRECT_CODE) {
        print_error(init_code, "Mutex init error");
        return EXCEPTION_CODE;
    }
    init_code = pthread_mutex_init(&mutex1, NULL);
    if (init_code != CORRECT_CODE) {
        print_error(init_code, "Mutex init error");
        return EXCEPTION_CODE;
    }
    init_code = pthread_mutex_init(&mutex2, NULL);
    if (init_code != CORRECT_CODE) {
        print_error(init_code, "Mutex init error");
        return EXCEPTION_CODE;
    }

    return CORRECT_CODE;
}

void destroy_mutexes() {
    int destroy_code;
    destroy_code = pthread_mutex_destroy(&startMutex);
    if (destroy_code != CORRECT_CODE) {
        print_error(destroy_code, "Mutex destroy error");
    }
    destroy_code = pthread_mutex_destroy(&mutex1);
    if (destroy_code != CORRECT_CODE) {
        print_error(destroy_code, "Mutex destroy error");
    }
    destroy_code = pthread_mutex_destroy(&mutex2);
    if (destroy_code != CORRECT_CODE) {
        print_error(destroy_code, "Mutex destroy error");
    }
}

void lock_mutex(pthread_mutex_t *mutex) {
    int lock_code = pthread_mutex_lock(mutex);
    if (lock_code != CORRECT_CODE) {
        print_error(lock_code, "Mutex lock error");
    }
}

void unlock_mutex(pthread_mutex_t *mutex) {
    int unlock_code = pthread_mutex_unlock(mutex);
    if (unlock_code != CORRECT_CODE) {
        print_error(unlock_code, "Mutex unlock error");
    }
}

int main(int argc, char **argv) {
    pthread_t thread;

    int init_code = initialize_mutexes();
    if (init_code != CORRECT_CODE) {
        destroy_mutexes();
        return EXCEPTION_CODE;
    }

    int lock_code = pthread_mutex_lock(&startMutex);
    if (lock_code != CORRECT_CODE) {
        print_error(lock_code, "Start mutex lock error")
        destroy_mutexes();
        return EXCEPTION_CODE;
    }

    int create_code = pthread_create(&thread, NULL, print_strings, NULL);
    if (create_code != CORRECT_CODE) {
        printf("Error №%d: %s\n", create_code, strerror(create_code));
    }

    lock_mutex(&mutex2);

    unlock_mutex(&startMutex);

    for (int i = 0; i < SRT_COUNT; ++i) {
        lock_mutex(&mutex1);
        printf("Main thread - %d\n", i);
        unlock_mutex(&mutex2);
    }

    int join_code = pthread_join(thread, NULL);
    if (join_code != CORRECT_CODE) {
        fprintf(stderr, "Thread joining error %d: %s\n", join_code, strerror(join_code));
    }

    destroy_mutexes();
    pthread_exit(NULL);
}
