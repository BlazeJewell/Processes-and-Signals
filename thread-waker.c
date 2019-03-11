#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>


sem_t *sem;

int main(void) {
	int numWakeup = 0;
	// call the function to open a named semaphore; the named semaphore has already been created so only 2 params are needed: nameOfYourSemaphore and 0 (zero)
	sem = sem_open("mySem", 0);

	// display the PID of the thread-waker process
	printf("thread-waker process, PID: %d \n", getpid());

	// do-while numWakeup != 0:
	do{
		// prompt User for the number of threads to wake-up
		printf("Enter the number of threads to wake-up (or 0 to quit): ");
		fflush( stdout );
		//Get number of threads (local var nThreads)
		scanf("%d", &numWakeup);
		// for numWakepidiup:
		for(int i=0;i<numWakeup;i++){
			// call the function to unlock the semaphore
			sem_post(sem);
		}

	}while(numWakeup != 0);

	// call function to close the semaphore
	sem_close(sem);

	// exit SUCCESS, end of main
	return EXIT_SUCCESS;
}
