#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define LINES_TO_PRINT_COUNT 10
#define CHILD "child"
#define PARENT "parent"



void printLines(char* str) {
    for (int i = 0; i < LINES_TO_PRINT_COUNT; i++) {
        printf("%s %d\n", str, i);
    }
}

void* threadFunction(void *param) {

    printLines(CHILD);

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thread;
    
    int err = pthread_create(&thread, NULL, threadFunction, NULL);

    switch (err) {
        case EAGAIN:
        case EINVAL:
        case EPERM:
            fprintf(stderr, "Thread creating error %d: %s\n", err, strerror(err));
            return EXCEPTION_EXIT_CODE;
    }

    printLines(PARENT);

    pthread_exit(NULL);
}

#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef LINES_TO_PRINT_COUNT
#undef CHILD
#undef PARENT
