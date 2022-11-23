#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define EXCEPTION_CODE 1
#define NUMBER_OF_SEMAPHORES 2

#define SRT_COUNT 10

sem_t sems[NUMBER_OF_SEMAPHORES];

typedef struct thread_arg {
    const char* text;
    int start;
} thread_arg;

void print_error(int return_code, char *additional_message) {
    fprintf(stderr, "%s %d: %s\n", additional_message, return_code, strerror(return_code));
}

int destroy_sems(int number) {
    for (int i = 0; i < number; i++) {
        int post_code = sem_post(&sems[i]);
        if (post_code != SUCCESS) {
            print_error(post_code, "Semaphore post error");
            return EXCEPTION_CODE;
        }
        int destroy_code = sem_destroy(&sems[i]);
        if (destroy_code != SUCCESS) {
            print_error(destroy_code, "Destroying semaphore error");
            return EXCEPTION_CODE;
        }
    }
    return SUCCESS;
}

int initialize_sems() {
    for (int i = 0; i < NUMBER_OF_SEMAPHORES; ++i) {
        int init_code = sem_init(&sems[i], 0, i);
        if (init_code != SUCCESS) {
            print_error(init_code, "Sem_init error");
            return init_code;
        }
    }
    return SUCCESS;
}

int semaphore_wait(int num) {
    int wait_code = sem_wait(&sems[num]);
    if (wait_code != SUCCESS) {
        print_error(wait_code, "Semaphore wait error");
        return EXCEPTION_CODE;
    }
    return SUCCESS;
}

int semaphore_post(int num) {
    int post_code = sem_post(&sems[num]);
    if (post_code != SUCCESS) {
        print_error(post_code, "Semaphore post error") ;
        return EXCEPTION_CODE;
    }
    return SUCCESS;
}

void* printTextInThread(void* args) {
    thread_arg* value = (thread_arg*)args;
    int this_sem = 0,
        next_sem = 0;

    for (int i = 0; i < SRT_COUNT; i++) {
        this_sem = (value->start + 1) % NUMBER_OF_SEMAPHORES;
        next_sem = (this_sem + 1) % NUMBER_OF_SEMAPHORES;
        int wait_code = semaphore_wait(this_sem);
        if (wait_code != SUCCESS) {
            return NULL;
        }

        printf("%s %d\n", value->text, i);

        int post_code = semaphore_post(next_sem);
        if (post_code != SUCCESS) {
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t thread;
    thread_arg newThread = { "Hello, I'm new thread", 1};
    thread_arg mainThread = { "Hello, I'm main thread", 0};
    int init_code = initialize_sems();
    if (init_code != SUCCESS) {
        destroy_sems(NUMBER_OF_SEMAPHORES);
        exit(init_code);
    }

    int create_code = pthread_create(&thread, NULL, printTextInThread, &newThread);
    if (create_code != SUCCESS) {
        print_error(create_code, "Creating thread error");
        destroy_sems(NUMBER_OF_SEMAPHORES);
        exit(create_code);
    }

    printTextInThread(&mainThread);

    int join_code = pthread_join(thread, NULL);
    if (join_code != SUCCESS) {
        print_error(join_code, "Joining thread error");
        destroy_sems(NUMBER_OF_SEMAPHORES);
        exit(join_code);
    }

    destroy_sems(NUMBER_OF_SEMAPHORES);
    return SUCCESS;
}