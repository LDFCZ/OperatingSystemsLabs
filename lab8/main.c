#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_OF_STEPS 200000000

#define NUM_OF_THREADS 1

#define BUF_SIZE 1024

#define CORRECT_CODE 0
#define EXCEPTION_CODE 1

#define MIN_NUM_OF_THREADS 1

#define NUM_OF_ARGS 2

#define MAX_NUM_OF_THREADS 30000

#define BASE 10

#define TERMINATING_SYMBOL_OF_STR '\0'

typedef struct partial_sum_args
{
    int start_index;
    int end_index;
    double result;
} partial_sum_args;

void print_error(int return_code, char *additional_message)
{
    fprintf(stderr, "%s %d: %s\n", additional_message, return_code, strerror(return_code));
}

void *calc_partial_sum(void *arg)
{

    partial_sum_args *args = (partial_sum_args *)arg;
    double partial_sum = 0;

    for (int i = args->start_index; i < args->end_index; ++i)
    {
        partial_sum += 1.0 / (i * 4.0 + 1.0);
        partial_sum -= 1.0 / (i * 4.0 + 3.0);
    }

    args->result = partial_sum;
    pthread_exit(NULL);
}

int join_threads_with_partial_sum(int num_of_threads, pthread_t *threads_id, partial_sum_args *threads_args, double *sum)
{
    int return_code = CORRECT_CODE;
    for (int thread_num = 0; thread_num < num_of_threads; ++thread_num)
    {
        int return_code = pthread_join(threads_id[thread_num], NULL);

        if (return_code != CORRECT_CODE)
        {
            print_error(return_code, "Join thread");
            return_code = EXCEPTION_CODE;
        }
        *sum += threads_args[thread_num].result;
    }
    return return_code;
}

int create_threads_for_partial_sum(int num_of_threads, pthread_t *threads_id, partial_sum_args *threads_args)
{

    int iteration_num_for_thread = NUM_OF_STEPS / num_of_threads;
    int num_of_additional_iterations = NUM_OF_STEPS % num_of_threads;

    for (int thread_num = 0; thread_num < num_of_threads; ++thread_num)
    {
        threads_args[thread_num].start_index = thread_num * iteration_num_for_thread;
        threads_args[thread_num].end_index = threads_args[thread_num].start_index + iteration_num_for_thread;
        if (thread_num < num_of_additional_iterations)
        {
            ++threads_args[thread_num].end_index;
        }

        int return_code = pthread_create(&threads_id[thread_num], NULL, calc_partial_sum, (void *)&threads_args[thread_num]);
        if (return_code != CORRECT_CODE)
        {
            return return_code;
        }
    }
    return CORRECT_CODE;
}

int is_valid_input(int num_of_args, char *num_of_threads_arg)
{

    if (num_of_args != NUM_OF_ARGS)
    {
        printf("Please write two arguments\nUsing: num_threads(more than 0 and less then 30 001)\n");
        return EXCEPTION_CODE;
    }

    char *endptr;
    long num_of_threads = strtol(num_of_threads_arg, &endptr, BASE);
    if (*endptr != TERMINATING_SYMBOL_OF_STR)
    {
        printf("Please write an integer\nUsing: num_threads(more than 0 and less then 30 001)\n");
        return EXCEPTION_CODE;
    }

    if (num_of_threads < MIN_NUM_OF_THREADS || num_of_threads > MAX_NUM_OF_THREADS)
    {
        printf("Please write number more than 0 and less than 30001\n");
        return EXCEPTION_CODE;
    }
    return CORRECT_CODE;
}

int calculate_pi(int num_of_threads, double *pi)
{

    pthread_t threads_id[num_of_threads];          
    partial_sum_args threads_args[num_of_threads]; 

    int return_code = create_threads_for_partial_sum(num_of_threads, threads_id, threads_args);
    if (return_code != CORRECT_CODE)
    {
        print_error(return_code, "Creating thread");
    }

    *pi = 0;

    return_code = join_threads_with_partial_sum(num_of_threads, threads_id, threads_args, pi);
    if (return_code != CORRECT_CODE)
    {
        return EXCEPTION_CODE;
    }

    *pi = *pi * 4.0;
    return CORRECT_CODE;
}

int main(int argc, char **argv)
{

    if (is_valid_input(argc, argv[NUM_OF_THREADS]) != CORRECT_CODE)
    {
        return EXIT_FAILURE;
    }

    int num_of_threads = (int)strtol(argv[NUM_OF_THREADS], NULL, BASE);

    double pi = 0;
    int return_code = calculate_pi(num_of_threads, &pi);
    if (return_code != CORRECT_CODE)
    {
        return EXIT_FAILURE;
    }

    printf("pi - %.15g \n", pi);

    return EXIT_SUCCESS;
}

#undef NUM_OF_STEPS

#undef NUM_OF_THREADS

#undef BUF_SIZE

#undef CORRECT_CODE
#undef EXCEPTION_CODE

#undef MIN_NUM_OF_THREADS

#undef NUM_OF_ARGS

#undef MAX_NUM_OF_THREADS

#undef BASE

#undef TERMINATING_SYMBOL_OF_STR
