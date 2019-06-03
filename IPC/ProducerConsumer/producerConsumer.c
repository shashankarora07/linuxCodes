#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>

#define BUFFER_LIMIT 10

int *buffer_Queue;
int buffer_value = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Signal_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t Signal_not_empty = PTHREAD_COND_INITIALIZER;

void *producer()
{
	while(1) {
		pthread_mutex_lock(&mutex);
		if (BUFFER_LIMIT == buffer_value) {
			pthread_cond_wait(&Signal_not_empty,&mutex);
		}
		printf("Producer produce : %d\n",buffer_value);
		buffer_Queue[buffer_value++] = buffer_value;
		pthread_cond_signal(&Signal_not_full);
		//pthread_exit("Producer thread exit");
		sleep(1);
		pthread_mutex_unlock(&mutex);
		
	}
}

void *consumer()
{
	while(1) {
		pthread_mutex_lock(&mutex);
		if (buffer_value == 0) {
			pthread_cond_wait(&Signal_not_full,&mutex);
		}
		printf("Consumer consumed : %d\n",--buffer_value);
		pthread_cond_signal(&Signal_not_empty);
		sleep(1);
		//pthread_exit("Consumer thread exit");
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char *argv[])
{
	pthread_t producerID, consumerID;
	char *ret;
	buffer_Queue = (int *)malloc(sizeof(int) * BUFFER_LIMIT);
	
	pthread_create(&producerID, NULL, producer, NULL);
	pthread_create(&consumerID, NULL, consumer, NULL);
	pthread_join(producerID,(void **)&ret);	
	pthread_join(consumerID,(void **)&ret);	

	return 0;
}
