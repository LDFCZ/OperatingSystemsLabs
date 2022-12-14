#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CORRECT_CODE 0
#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define EXCEPTION_CODE -1

#define COEFFIECIENT 100000
#define MAX_NUM_STR 100

void *printString(void *args)
{
    int usleep_err = usleep(COEFFIECIENT * strlen((char *)args));
    if (usleep_err != CORRECT_CODE)
    {
        fprintf(stderr, "usleep error %d: %s\n", usleep_err, strerror(usleep_err));
    }
    printf("%s", (char *)args);
    return NULL;
}

void freeStrings(char **strings, int strCount)
{
    for (int i = 0; i < strCount; i++)
    {
        free(strings[i]);
    }
}

int readStrings(char **strings, int *strCount)
{
    int readCount = 2;
    int idx = 0;
    size_t strlen = 200;

    printf("Enter strings to sort:\n");
    while (readCount > 1 && idx < 100)
    {
        strings[idx] = (char *)malloc(sizeof(char) * strlen);
        if (strings[idx] == NULL)
        {
            fprintf(stderr, "Malloc error: %s\n", strerror(errno));
            *strCount = idx;
            return EXCEPTION_CODE;
        }

        readCount = getline(&strings[idx], &strlen, stdin);
        if (readCount == EXCEPTION_CODE) 
        {
            fprintf(stderr, "Getline error: %s\n", strerror(errno));
            *strCount = idx;
            return EXCEPTION_CODE;
        }

        idx++;
        if (readCount == 1) idx--; 
    }
    *strCount = idx;
    return CORRECT_CODE;
}

void doCleanUp(char **strings, int strCount, pthread_t *threads)
{
    freeStrings(strings, strCount);
    free(threads);
}

int main()
{
    char *strings[MAX_NUM_STR];
    int strCount;
    int read_str_err = readStrings(strings, &strCount);
    if (read_str_err == EXCEPTION_CODE)
    {
        freeStrings(strings, strCount);
        return EXCEPTION_EXIT_CODE;
    }

    printf("Sorting your strings. . .\n");

    pthread_t *threads = malloc(sizeof(pthread_t) * strCount);
    if (threads == NULL)
    {
        fprintf(stderr, "Malloc error: %s\n", strerror(errno));
        freeStrings(strings, strCount);
        return EXCEPTION_EXIT_CODE;
    }

    for (int i = 0; i < strCount; i++)
    {
        int create_err = pthread_create(&threads[i], NULL, printString, strings[i]);
        if (create_err != CORRECT_CODE)
        {
            fprintf(stderr, "Thread creating error %d: %s\n", create_err, strerror(create_err));
            break;
        }
    }

    for (int i = 0; i < strCount; i++)
    {
        int join_err = pthread_join(threads[i], NULL);
        if (join_err != CORRECT_CODE)
        {
            fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
        }
    }

    doCleanUp(strings, strCount, threads);
    return CORRECT_EXIT_CODE;
}

#undef CORRECT_CODE
#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef EXCEPTION_CODE

#undef COEFFIECIENT
#undef MAX_NUM_STR
