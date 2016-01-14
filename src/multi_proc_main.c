#include "multi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <unistd.h>

int do_something(int shm_id)
{
    if (shm_id <= 0) {
        printf("Argument shm_id is invalid.\n");
        return ERR;
    }
    char *shm_addr = NULL;
    /* Shared memory map */
    if ((shm_addr = shmat(shm_id, NULL, 0)) == (void *) -1) {
        printf("Shared memory mapping faild.\n");
        return ERR;
    }
    int local_counter = *shm_addr;
    local_counter++;
    *shm_addr = local_counter;

    /* Shared memory unmap */
    if (shmdt(shm_addr) < 0) {
        printf("Shared memory unmapping faild.\n");
        return ERR;
    }

    int r = rand();
    int sleeping_sec = r % 10;

    printf("Job %d started, sleep sec: %d.\n", local_counter,
           sleeping_sec);
    sleep(sleeping_sec);
    printf("Job %d ended.\n", local_counter);

    return OK;
}

int main(int argc, const char **argv)
{
    int shm_id = 0;
    int i = 0;
    pid_t pid = 0;

    if ((shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0660)) <= 0) {
        printf("Shared memory get faild.\n");
        return EXIT_FAILURE;
    }

    char *shm_addr = NULL;
    /* Shared memory map */
    if ((shm_addr = shmat(shm_id, NULL, 0)) == (void *) -1) {
        printf("Shared memory mapping faild. %d, [%s]\n",
               errno, strerror(errno));
        return ERR;
    }
    memset(shm_addr, 0x00, sizeof(int));
    /* Shared memory unmap */
    if (shmdt(shm_addr) < 0) {
        printf("Shared memory unmapping faild.\n");
        return ERR;
    }

    pid_t child_pids[PROC_MAX];
    memset(child_pids, 0x00, sizeof(pid_t) * PROC_MAX);

    printf("Proc max: %d\n", PROC_MAX);
    int is_parent = TRUE;
    for (i = 0; i < PROC_MAX; i++) {
        printf("Proc count: %d\n", i);

        if ((pid = fork()) == 0) {
            /* Child process start */
            is_parent = FALSE;
            printf("Child process. My pid is %d, my parent's pid is %d.\n",
                   getpid(), getppid());
            do_something(shm_id);
            /* Child process end */
            break;
        } else {
            printf("Parent process. My pid is %d, my child's pid is %d.\n",
                   getpid(), pid);
            child_pids[i] = pid;
        }
    }

    if (is_parent) {
        int child_pid = 0;
        int child_pid_status = 0;
        pid_t returned_pid = 0;
        for (i = 0; i < PROC_MAX; i++) {
            child_pid = child_pids[i];
            if ((returned_pid =
                 waitpid(child_pid, &child_pid_status, 0)) < 0) {
                printf("waitpid failed.\n");
                return ERR;
            }
        }
    }

    return EXIT_SUCCESS;
}
