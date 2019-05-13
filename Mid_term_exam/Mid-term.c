#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

int data = 0;
sem_t sem1;

void* thread_function1(){
	for(int i=0; i<100000; i++){
		sem_wait(&sem1);
		data += 1;
		sem_post(&sem1);
	}
    pthread_exit("Finished thread1");
}

void* thread_function2(){
	for(int i=0; i<100000; i++){
		sem_wait(&sem1);
		data -= 1;
		sem_post(&sem1);
	}
    pthread_exit("Finished thread2");
}

int main(){

	sem_init(&sem1, 0, 1);	

	pthread_t pthread1;
	pthread_t pthread2;
    int result;
    void* thread_result1;
    void* thread_result2;

	result = pthread_create(&pthread1, NULL, thread_function1, NULL);
    if(result != 0){
        perror("Create thread failed!");
        exit(1);
    }
	result = pthread_create(&pthread2, NULL, thread_function2, NULL);
    if(result != 0){
        perror("Create thread failed!");
        exit(1);
    }

	result = pthread_join(pthread1, &thread_result1);
    if(result != 0){
        perror("Join thread failed!");
        exit(1);
    }
	result = pthread_join(pthread2, &thread_result2);
    if(result != 0){
        perror("Join thread failed!");
        exit(1);
    }

	printf("%s\n", (char*)thread_result1);
	printf("%s\n", (char*)thread_result2);

	printf("result: %d\n", data);

	sem_destroy(&sem1);		

	sleep(1);

	return 0;
}
