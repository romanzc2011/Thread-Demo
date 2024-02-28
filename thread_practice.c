#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// pointer to define array with randomly generated integers
int *a;
int i;
int sum;

void *runner(void *param);

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    fprintf(stderr, "usage: %s <integer value>\n", argv[0]);
    return -1;
  }

  // How many threads to create
  int N = atoi(argv[1]);

  if(N <= 0)
  {
    fprintf(stderr, "%d must be > 0\n", N);
    return -1;
  }
  
  pthread_t worker[N];
  pthread_attr_t attr; // Attributes for threads

  // Dynamic memory to create an array
  a = (int *)malloc(N *sizeof(int));

  if(a == NULL)
  {
    fprintf(stderr, "Memo ry allocation failed!.\n");
    return 1;
  }

  // seed the random number generator
  srandom((unsigned)time(NULL));

  // init default thread attr
  pthread_attr_init(&attr);

  // Create threads in for loop
  for(i = 0; i < N; i++)
  {
      worker[i] = pthread_create(&worker[i], &attr, runner, (void *)i);
  }

  // Wait for thread to exit
  for(i = 0; i < N; i++)
  {
      pthread_join(worker[i], NULL);
  }

  for(i = 0; i < N; i++)
  {
      sum += a[i];
      printf("%d\n", a[i]);
  }

  printf("sum = %d\n", sum);

  free(a);
  return 0;
}

void *runner(void *param)
{
    int thread_no = (int)param;
    int i;

    for(i = 0; i < 1000; i++)
    {
        sum++;
        sum--;
    }

    printf("This is thread %d, I increased sum by 1 and decreased sum by 1.\n", thread_no);
    pthread_exit(0);
}
