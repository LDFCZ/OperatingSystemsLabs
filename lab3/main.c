#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define CORRECT_EXIT_CODE 0
#define EXCEPTION_EXIT_CODE 1

#define CORRECT_CODE 0
#define EXCEPTION_CODE 1

#define NUM_OF_THREADS 4

void *print_message(void *arg) {
    char **message = (char **)arg;
    for (int str_num = 0; message[str_num] != NULL; ++str_num) {
	printf("%s", message[str_num]);
    }
    return NULL;
}

int main() {
    
    pthread_t threads_id[NUM_OF_THREADS];
    char *messages[][NUM_OF_THREADS] = {
	    {"Thread 1, line 1\n", "Thread 1, line 2\n", "Thread 1, line 3\n", NULL},
	    {"Thread 2, line 1\n", "Thread 2, line 2\n", "Thread 2, line 3\n", NULL},
	    {"Thread 3, line 1\n", "Thread 3, line 2\n", "Thread 3, line 3\n", NULL},
	    {"Thread 4, line 1\n", "Thread 4, line 2\n", "Thread 4, line 3\n", NULL}
	};

    for (int thread_num = 0; thread_num < NUM_OF_THREADS; ++thread_num) {
	    int create_err = pthread_create(&threads_id[thread_num], NULL, print_message, &messages[thread_num]);
    	if (create_err != CORRECT_CODE) {
            fprintf(stderr, "Thread creating error %d: %s\n", create_err, strerror(create_err));
            return EXCEPTION_EXIT_CODE;
        }
    }

    for (int thread_num = 0; thread_num < NUM_OF_THREADS; ++thread_num) {
	    int join_err = pthread_join(threads_id[thread_num], NULL);
	    if (join_err != CORRECT_CODE) {
            fprintf(stderr, "Thread joining error %d: %s\n", join_err, strerror(join_err));
            return EXCEPTION_EXIT_CODE;
        }
    }
    pthread_exit(NULL);
}

#undef CORRECT_EXIT_CODE
#undef EXCEPTION_EXIT_CODE

#undef CORRECT_CODE
#undef EXCEPTION_CODE

#undef NUM_OF_THREADS