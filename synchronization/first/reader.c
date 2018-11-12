#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "write_helper.h"

int main ()
{
	key_t shm_key = 12345;
	const int shm_size = 1024;
	int slock_fd = -1;
	void *mapped = NULL;
	pthread_mutex_t *lk = NULL;
	pthread_mutex_t dummy;
	void *dummy2 = NULL;

	dummy2 = malloc(sizeof(pthread_mutex_t));

	int shm_id;
	char* shmaddr, *ptr;
	char* shared_memory[3];
	int *p;

	/* Allocate a shared memory segment. */
	shm_id = shmget (shm_key, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);

	/* Attach the shared memory segment. */
	shmaddr = (char*) shmat (shm_id, 0, 0);

	printf ("shared memory attached at address %p\n", shmaddr);
	slock_fd = slock_mutex_create("slock_file1");
	if (slock_fd == -1) {
		printf("\nReader failed to create mutex.\n");
	}

	printf("\nReader received sfd = %d\n", slock_fd);

	mapped = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t), PROT_WRITE | PROT_READ, MAP_SHARED, slock_fd, 0);
	if (mapped == MAP_FAILED) {
		printf("\nReader failed to mmap the shared region.\n");
		return -1;
	}

//	printf("\nREADER string = %s\n", (char *) mapped);

	printf("\nCalling pthread_mutex_lock\n");
	if ((pthread_mutex_lock(mapped)) != 0) {
		printf("\nReader failed to aqiure the lock.\n");
		return -1;
	}

	/* Start to read data. */
	p = (int *)shmaddr;
	ptr = shmaddr + sizeof (int) * 2;
	shared_memory[0] = ptr;
	ptr += *p++;
	shared_memory[1] = ptr;
	ptr += *p;
	shared_memory[2] = ptr;
	printf ("0=%s\n", shared_memory[0]);
	printf ("1=%s\n", shared_memory[1]);
	printf ("2=%s\n", shared_memory[2]);

	if (pthread_mutex_unlock(mapped)) {
		printf("\nReader failed to release lock.\n");
		return -1;
	}

	/* Detach the shared memory segment. */
	shmdt (shmaddr);
	return 0;
}
