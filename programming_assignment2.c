
basovo/*
    Name: Roman Campbell
    Course: CSC 690
*/

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// ##################################################################################################
// Preprocessors
#define NUM_THREADS 3

// ##################################################################################################
// Variables
int N,VAL,i,j,min,max,avg_result;
int min_thread,max_thread,avg_thread;
int avg_result;
int *int_array;
pthread_mutex_t mutex;

// ##################################################################################################
// Functions
void *get_avg(void *param);
void *get_min(void *param);
void *get_max(void *param);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "usage: %s <at least one integer as input>\n", argv[0]);
        return 1; 
    }

    if(argc >= 2)
    {
        N = argc - 1;
        int_array = (int *)malloc(N *sizeof(int));
    }

    if(int_array == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Assign args to int_array
    for(i = 0; i < N; i++)
    {
        VAL = atoi(argv[i+1]);
        int_array[i] = VAL;
    }

    // ###################################################################################################
    // THREAD SECTION
    // ###################################################################################################

    // Create threads for operations
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    pthread_mutex_init(&mutex,NULL);
    pthread_attr_init(&attr);

    // Create threads for the min max and avg ops
    avg_thread = pthread_create(&threads[0],&attr,get_avg,(void *)0); 
    min_thread = pthread_create(&threads[1],&attr,get_min,(void *)1);  
    max_thread = pthread_create(&threads[2],&attr,get_max,(void *)2); 

    if(avg_thread != 0|| min_thread != 0 || max_thread != 0)
    {
        fprintf(stderr, "Error creating threads\n");
        free(int_array);
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("The average value is %d\n", avg_result);
    printf("The minimum value is %d\n",min);
    printf("The maximum value is %d\n",max);

    free(int_array);
    return 0;
}

// AVERAGE
void *get_avg(void *param)
{
    int thread_no = (int)param;
    int i;
    int sum;
          
    pthread_mutex_lock(&mutex);
    for(i = 0; i < N; i++)
    {
        sum+= int_array[i];
    }
    avg_result = sum / N;
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}

// MAX
void *get_max(void *param)
{
    int thread_no = (int)param;
    int i;

    max = int_array[0];

    pthread_mutex_lock(&mutex);
    for(i = 1; i < N; i++)
    {
        if(int_array[i] > max)
        {
            max = int_array[i];
        }
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}

// MIN
void *get_min(void *param)
{
    int thread_no = (int)param;
    int i;

    min = int_array[0];

    pthread_mutex_lock(&mutex);
    for(i = 1; i < N; i++)
    {
        if(int_array[i] < min)
        {
            min = int_array[i];
        }
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}