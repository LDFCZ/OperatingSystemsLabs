#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define CORRECT_CODE 0
#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

pthread_mutex_t mutex1, mutex2, startMutex;

void print_error(int return_code, char *additional_message)
{
    fprintf(stderr, "%s %d: %s\n", additional_message, return_code, strerror(return_code));
}

void *print_strings(void *param)
{
    pthread_mutex_lock(&startMutex);
    for (int i = 0; i < 5; ++i)
    {
        pthread_mutex_lock(&mutex2);
        printf("Thread - %d\n", i);
        pthread_mutex_unlock(&mutex1);
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t thread;

    pthread_mutex_init(&startMutex, NULL);
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_mutex_lock(&startMutex);
    int err = pthread_create(&thread, NULL, print_strings, NULL);
    if (err)
    {
        printf("Error №%d: %s\n", err, strerror(err));
    }
    pthread_mutex_lock(&mutex2);

    pthread_mutex_unlock(&startMutex);
    for (int i = 0; i < 5; ++i)
    {
        pthread_mutex_lock(&mutex1);
        printf("Main thread - %d\n", i);
        pthread_mutex_unlock(&mutex2);
    }

    int join_err = pthread_join(thread, NULL);
    if (join_err != CORRECT_CODE) {
        fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
    }

    pthread_mutex_destroy(&startMutex);
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    pthread_exit(NULL);
}
