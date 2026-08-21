#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

void *thread_worker(void *arg) {
  int thread_id = *(int *)arg;
  printf("thread %d started\n", thread_id);
  sleep(1);
  printf("thread %d completed\n", thread_id);
  return NULL;
}

int main(int argc, char *argv[]) {
  printf("POSIX threads\n");

  pthread_t threads[NUM_THREADS];
  int threads_ids[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    threads_ids[i] = i;
    if (pthread_create(&threads[i], NULL, thread_worker, &threads_ids[i])) {
      perror("pthread_create");
      exit(1);
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
      exit(1);
    }
  }

  return 0;
}
