#include "multi.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int counter = 0;
pthread_mutex_t lock;

void* do_something(void *arg)
{
    int local_counter = 0;

    pthread_mutex_lock(&lock);
    counter++;
    local_counter = counter;
    pthread_mutex_unlock(&lock);

    int r = rand();
    int sleeping_sec = r % 10;

    printf("Job %d started, sleep sec: %d.\n", local_counter, sleeping_sec);
    sleep(sleeping_sec);
    printf("Job %d ended.\n", local_counter);

    return NULL;
}

int main(int argc, const char **argv)
{
    int err = 0;
    int i = 0;
    pthread_t thread_id[THREAD_MAX];

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init faild.");
        return EXIT_FAILURE;
    }

    for (i = 0; i < THREAD_MAX; i++) {
        err = pthread_create(&(thread_id[i]), NULL, &do_something, NULL);
        if (err != 0) {
            printf("Can't create thread : [%s]", strerror(err));
        }
        /* pthread_join(thread_id[i], NULL); */
    }
    for (i = 0; i < THREAD_MAX; i++) {
        pthread_join(thread_id[i], NULL);
    }

    if (pthread_mutex_destroy(&lock) != 0) {
        printf("Mutex destroy faild.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
