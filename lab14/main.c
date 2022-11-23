#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define NUMBER_OF_SEMAPHORES 2

#define SRT_COUNT 10

sem_t sems[NUMBER_OF_SEMAPHORES];

typedef struct thread_arg {
    const char* text;
    int start;
} thread_arg;

int error_check(int code, char* inscription) {
    if (code != SUCCESS) {
        perror(inscription);
        return code;
    }
    return SUCCESS;
}

int destroy_sems(int number) {
    for (int i = 0; i < number; i++) {
        errno = sem_post(&sems[i]);
        if (error_check(errno, "Semaphore post error") != SUCCESS) {
            return errno;
        }
        errno = sem_destroy(&sems[i]);
        if (error_check(errno, "Destroying semaphore error") != SUCCESS) {
            return errno;
        }
    }
    return SUCCESS;
}

int initialize_sems() {
    for (int i = 0; i < NUMBER_OF_SEMAPHORES; ++i) {
        errno = sem_init(&sems[i], 0, i);
        if (error_check(errno, "Sem_init error") != SUCCESS) {
            destroy_sems(i);
            return errno;
        }
    }
    return SUCCESS;
}
int semaphore_wait(int num) {
    errno = sem_wait(&sems[num]);
    if (error_check(errno, "Semaphore wait error") != SUCCESS) {
        return errno;
    }
    return SUCCESS;
}

int semaphore_post(int num) {
    errno = sem_post(&sems[num]);
    if (error_check(errno, "Semaphore post error") != SUCCESS) {
        return errno;
    }
    return SUCCESS;
}

void* printTextInThread(void* args) {
    thread_arg* value = (thread_arg*)args;
    errno = SUCCESS;
    int this_sem = 0,
        next_sem = 0;

    for (int i = 0; i < SRT_COUNT; i++) {
        this_sem = (value->start + 1) % NUMBER_OF_SEMAPHORES;
        next_sem = (this_sem + 1) % NUMBER_OF_SEMAPHORES;
        errno = semaphore_wait(this_sem);
        if (error_check(errno, "Semaphore wait error") != SUCCESS) {
            return NULL;
        }

        printf("%s %d\n", value->text, i);

        errno = semaphore_post(next_sem);
        if (error_check(errno, "Semaphore post error") != SUCCESS) {
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t thread;
    thread_arg newThread = { "Hello, I'm new thread", 1};
    thread_arg mainThread = { "Hello, I'm main thread", 0};
    errno = initialize_sems();
    if (errno != SUCCESS) {
        exit(errno);
    }

    errno = pthread_create(&thread, NULL, printTextInThread, &newThread);
    if (error_check(errno, "Creating thread error") != SUCCESS) {
        destroy_sems(NUMBER_OF_SEMAPHORES);
        exit(errno);
    }

    printTextInThread(&mainThread);

    errno = pthread_join(thread, NULL);
    if (error_check(errno, "Joining thread error") != SUCCESS) {
        destroy_sems(NUMBER_OF_SEMAPHORES);
        exit(errno);
    }

    destroy_sems(NUMBER_OF_SEMAPHORES);
    return SUCCESS;
}