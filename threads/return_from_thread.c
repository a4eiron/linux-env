#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int input;
} thread_data_t;

void *square(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
  int *result = malloc(sizeof(int));
  *result = data->input * data->input;
  return (void *)result;
}

int main() {
  pthread_t t;
  thread_data_t data;
  data.input = 10;

  if (pthread_create(&t, NULL, square, &data.input) != 0) {
    perror("pthread_create");
    exit(1);
  }

  int *result;
  if (pthread_join(t, (void **)&result) != 0) {
    perror("pthread_join");
    exit(1);
  }

  printf("returned result (main): %d\n", *result);
  free(result);

  return 0;
}
