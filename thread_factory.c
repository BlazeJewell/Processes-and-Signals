#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

volatile sig_atomic_t handlerCalled = 0;
sem_t *sem;
pid_t pid;

//this function is called when process receives SIGUSR1 signal (registration was done in main()); set integer flag to 1
void sigusr1_handler(int sig) {
	handlerCalled = 1;
}

void * childThread(void * arg){
	//print message that thread has been created; display thread ID
	printf("A thread has created, ID: %d \n", pthread_self() );

	while(1){
		// wait on the semaphore
		sem_wait(sem); // the latex protector
		// print message the thread has unblocked; display thread ID
		printf("Child %d has been unblocked\n", pthread_self());
		// sleep for 5 secs (simulates work being done by thread)
		sleep(5);
	}
	return NULL;
}

int main(void) {
	int nThreads = 0;
	int i = 0;
	struct sigaction sa;

	// Use  ‘sigemptyset()’ and ’sigaction()’ to register a handler function to intercept SIGUSR1 signals; on error: call perror and exit
	sa.sa_handler = sigusr1_handler;
	sa.sa_flags = 0; // or SA_RESTART
	sigemptyset(&sa.sa_mask);

	// Install your signal using sigaction( ), using the signal type SIGUSR1
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		return 1;
	}

	// Prompt User for number of threads to create
	printf("Enter the number of child threads: ");
	//Get number of threads (local var nThreads)
	scanf("%d", &nThreads);

	// call the function to open a named semaphore; 
	pthread_attr_t attr;

	sem = sem_open("mySem", O_CREAT, S_IWOTH, 0);

	// for nThreads:
	for(i=0;i<nThreads;i++){
		// call the pthread function to initializes attr with all default thread attributes
		pthread_attr_init(&attr);

		// call the pthread function to create a new thread (params 1 & 4 are NULL)
		pthread_create(NULL, &attr, &childThread, NULL);

		// call the pthread function to destroy attr
		pthread_attr_destroy(&attr);
	} // loop continuously until integer flag is set to 1
	

	while (!handlerCalled){
	}
	// call function to close the semaphore
	sem_close(sem);
	//sem_destroy(&sem);
	// exit SUCCESS // end of main
	return 0;
}
