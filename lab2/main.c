#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define CORRECT_CODE 0
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

    int create_err = pthread_create(&thread, NULL, threadFunction, NULL); 
    if (create_err != CORRECT_CODE) {
        fprintf(stderr, "Thread creating error %d: %s\n", create_err, strerror(create_err));
        return EXCEPTION_EXIT_CODE;
    }

    int join_err = pthread_join(thread, NULL);
    if (join_err != CORRECT_CODE) {
        fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
        return EXCEPTION_EXIT_CODE;
    }

    printLines(PARENT);

    return CORRECT_EXIT_CODE;
}

#undef CORRECT_CODE
#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef LINES_TO_PRINT_COUNT
#undef CHILD
#undef PARENT
