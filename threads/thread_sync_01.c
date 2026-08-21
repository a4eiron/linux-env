#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>

static long long counter = 0;
static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
static pthread_rwlock_t rwmu = PTHREAD_RWLOCK_INITIALIZER;

void *incr(void *arg) {
  int id = (int)(intptr_t)arg;
  for (int i = 0; i < 100000; i++) {
    if (pthread_mutex_lock(&mu) != 0) {
      perror("pthread_mutex_lock");
      exit(1);
    }
    counter++;
    if (pthread_mutex_unlock(&mu) != 0) {
      perror("pthread_mutex_unlock");
      exit(1);
    }
  }
  return NULL;
}

void locks1() {
  pthread_t t1, t2, t3;

  pthread_create(&t1, NULL, incr, (void *)(intptr_t)1);
  pthread_create(&t2, NULL, incr, (void *)(intptr_t)2);
  pthread_create(&t3, NULL, incr, (void *)(intptr_t)3);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);

  printf("counter: %lld\n", counter);
}

void *reader(void *arg) {
  int id = (int)(intptr_t)arg;
  for (int i = 0; i < 20; i++) {
    pthread_rwlock_rdlock(&rwmu);
    printf("thread: %d reader: %lld\n", id, counter);
    pthread_rwlock_unlock(&rwmu);
  }
  return NULL;
}

void *writer(void *arg) {
  for (int i = 0; i < 100000; i++) {
    pthread_rwlock_wrlock(&rwmu);
    counter++;
    pthread_rwlock_unlock(&rwmu);
  }
  return NULL;
}

void locks2() {
  pthread_t t1, t2, t3, t4;
  pthread_create(&t1, NULL, reader, (void *)(intptr_t)1);
  pthread_create(&t2, NULL, writer, NULL);
  pthread_create(&t3, NULL, reader, (void *)(intptr_t)2);
  pthread_create(&t4, NULL, writer, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);

  printf("counter: %lld\n", counter);
}

int main(int argc, char *argv[]) {
  // locks1();
  // pthread_mutex_destroy(&mu);
  // locks2();
  // pthread_rwlock_destroy(&rwmu);

  return 0;
}
