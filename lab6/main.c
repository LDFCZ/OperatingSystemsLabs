#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define EXCEPTION_CODE -1

#define COEFFIECIENT 0.5
#define MAX_NUM_STR 100

void* printString(void *args) {
    sleep(COEFFIECIENT * strlen((char*)args));
    printf("%s", (char*)args);
    return NULL;
}

void freeStrings(char **strings, int strCount) {
    for (int i = 0; i < strCount; i++) {
        free(strings[i]);
    }
    free(strings);
}

// Returns number of strings read or -1 if something goes wrong
int readStrings(char **strings) {
    int readCount = 2;
    int idx = 0;
    size_t strlen = 200;

    printf("Enter strings to sort:\n");
    while (readCount > 1) {
        strings[idx] = (char*)malloc(sizeof(char) * strlen);
        if (strings[idx] == NULL) {
            fprintf(stderr, "Malloc error\n");
            freeStrings(strings, idx);
            return EXCEPTION_CODE;
        }

        readCount = getline(&strings[idx], &strlen, stdin);
        idx++;

        if (readCount == 1){
            free(strings[idx]);
            idx--;
        }
    }
    return idx;
}



void doCleanUp(char **strings, int strCount, pthread_t *threads) {
    freeStrings(strings, strCount);
    free(threads);
}

int main() {

    char** strings = (char**)malloc(sizeof(char *) * MAX_NUM_STR);
    if (strings == NULL) {
        fprintf(stderr, "Malloc error\n");
        return EXCEPTION_EXIT_CODE;
    }

    int strCount = readStrings(strings);
    if (strCount == -1) {
        return EXCEPTION_EXIT_CODE;
    }

    printf("Sorting your strings. . .\n");

    pthread_t* threads = malloc(sizeof(pthread_t) * strCount);
    if (threads == NULL) {
        fprintf(stderr, "Malloc error\n");
        freeStrings(strings, strCount);
        return EXCEPTION_EXIT_CODE;
    }

    for (int i = 0; i < strCount; i++) {
        int create_err = pthread_create(&threads[i], NULL, printString, strings[i]); 
        switch (create_err) {
            case EAGAIN:
            case EINVAL:
            case EPERM:
                fprintf(stderr, "Thread creating error %d: %s\n", create_err, strerror(create_err));
                doCleanUp(strings, strCount, threads);
                return EXCEPTION_EXIT_CODE;
        }
    }

    for (int i = 0; i < strCount; i++) {
        int join_err = pthread_join(threads[i], NULL);
        switch (join_err) {
            case EDEADLK:
            case EINVAL:
            case ESRCH:
                fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
                doCleanUp(strings, strCount, threads);
                return EXCEPTION_EXIT_CODE;
        }
    }

    doCleanUp(strings, strCount, threads);
    return CORRECT_EXIT_CODE;
}

#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef EXCEPTION_CODE

#undef COEFFIECIENT
#undef MAX_NUM_STR
