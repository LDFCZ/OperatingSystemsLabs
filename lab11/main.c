#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define EXCEPTION_CODE 1
#define CORRECT_CODE 0
#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define SRT_COUNT 10

#define COUNT_MUTEXES 3

pthread_mutex_t mutex_array[COUNT_MUTEXES];
bool ready = false;

typedef struct thread_arg {
    const char* text;
    int count;
} thread_arg;



void print_error(int return_code, char *additional_message) {
    fprintf(stderr, "%s %d: %s\n", additional_message, return_code, strerror(return_code));
}

void lock_mutex(int i) {
    int lock_code = pthread_mutex_lock(&mutex_array[i]);
    if (lock_code != CORRECT_CODE) {
        print_error(lock_code, "Mutex lock error");
    }
}

void unlock_mutex(int i) {
    int unlock_code = pthread_mutex_unlock(&mutex_array[i]);
    if (unlock_code != CORRECT_CODE) {
        print_error(unlock_code, "Mutex unlock error");
    }
}

/*void *print_strings(void *param) {
    int lock_code = pthread_mutex_lock(&startMutex);
    if (lock_code != CORRECT_CODE) {
        print_error(lock_code, "Start mutex lock error");
        pthread_exit(NULL);
    }
    unlock_mutex(&startMutex);

    for (int i = 0; i < SRT_COUNT; ++i) {
        lock_mutex(&mutex2); 
        printf("Thread - %d\n", i);
        unlock_mutex(&mutex1);
    }
    unlock_mutex(&mutex2);
    
    pthread_exit(NULL);
}*/

/*int initialize_mutexes() {
    
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
   
}*/

int initialize_mutexes() {
    pthread_mutexattr_t attr;
    int code = pthread_mutexattr_init(&attr);
    if (code != CORRECT_CODE) {
        print_error(code, "Mutex attributes could not be created");
        return EXCEPTION_CODE;
    }

    code = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    if (code != CORRECT_CODE) {
        print_error(code, "Mutex attribute type could not be set");
        return EXCEPTION_CODE;
    }

    for (int i = 0; i < COUNT_MUTEXES; ++i) {
        code = pthread_mutex_init(&mutex_array[i], &attr);
        if (code != CORRECT_CODE) {
            print_error(code, "Mutex init error");
            return EXCEPTION_CODE;
        }
    }

    return CORRECT_CODE;
}

/*void destroy_mutexes() {

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
}*/

void destroy_mutexes() {
    for (int i = 0; i < COUNT_MUTEXES; ++i) {
        int code = pthread_mutex_destroy(&mutex_array[i]);
        if (code != CORRECT_CODE) {
            print_error(code, "mutex could not  be destroy");
        }
    }
}

void* print_text_in_thread(void* args) {
    thread_arg* value = (thread_arg*)args;
    int thisMutex = 0;
    int nextMutex = 0;
    if (!ready) {
        thisMutex = 2;
        lockMutex(thisMutex);
        ready = true;
    }
    for (int i = 0; i < value->count; ++i) {
        nextMutex = (thisMutex + 1) % COUNT_MUTEXES;
        lockMutex(nextMutex);
        printf("%d %s\n", i, value->text);
        unlockMutex(thisMutex);
        thisMutex = nextMutex;
    }
    unlockMutex(thisMutex);
    return NULL;
}


int main(int argc, char **argv) {
    pthread_t thread;

    thread_arg new_thread = { "Hello, I'm new thread\n", 10 };
    thread_arg main_thread = { "Hello, I'm main thread\n", 10 };

    int init_code = initialize_mutexes();
    if (init_code != CORRECT_CODE) {
        destroy_mutexes();
        return EXCEPTION_CODE;
    }

    int lock_code = pthread_mutex_lock(&mutex_array[0]);
    if (lock_code != CORRECT_CODE) {
        print_error(lock_code, "Start mutex lock error");
        destroy_mutexes();
        return EXCEPTION_CODE;
    }

    int create_code = pthread_create(&thread, NULL, print_text_in_thread, (void*)&new_thread);
    if (create_code != CORRECT_CODE) {
        printf("Error №%d: %s\n", create_code, strerror(create_code));
        print_error(create_code, "Create error ");
    }

    while (ready != true) {};

    print_text_in_thread((void*)&main_thread)

    int join_code = pthread_join(thread, NULL);
    if (join_code != CORRECT_CODE) {
        print_error(join_code, "Thread joining error ");
    }

    destroy_mutexes();
    return CORRECT_EXIT_CODE;
}
