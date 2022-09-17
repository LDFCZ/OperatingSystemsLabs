#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define CORRECT_CODE 0
#define EXCEPTION_CODE 1

void* printStringArray(void *param) {
    char **stringArray = (char**)param;
    for (char **currentString = stringArray; *currentString != NULL; ++currentString) {
        printf("%s\n", *currentString);
    }
    return NULL;
}

int errorHandling(int err) {
    switch (err) {
        case EAGAIN:
        case EINVAL:
        case EPERM:
            fprintf(stderr, "Thread creating error %d: %s\n", err, strerror(err));
            return EXCEPTION_CODE;
    }
    return CORRECT_CODE;
}

int main(int argc, char **argv) {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    char *stringArray1[] = {"1", "2", "3", NULL};
    char *stringArray2[] = {"4", "5", "6", "7", "8", NULL};
    char *stringArray3[] = {"9", "10", NULL};
    char *stringArray4[] = {"11", "12", "13", NULL};

    int err = pthread_create(&thread1, NULL, printStringArray, stringArray1);
    if (errorHandling(err)) return EXCEPTION_EXIT_CODE;

    err = pthread_create(&thread2, NULL, printStringArray, stringArray2);
    if (errorHandling(err)) return EXCEPTION_EXIT_CODE;

    err = pthread_create(&thread3, NULL, printStringArray, stringArray3);
    if (errorHandling(err)) return EXCEPTION_EXIT_CODE;

    err = pthread_create(&thread4, NULL, printStringArray, stringArray4);
    if (errorHandling(err)) return EXCEPTION_EXIT_CODE;
    
    pthread_exit(NULL);
}

#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef CORRECT_CODE
#undef EXCEPTION_CODE
