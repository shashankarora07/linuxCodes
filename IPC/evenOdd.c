#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>

#define LOOP_LIMIT 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
int count = 0;

void *evenFun(void *data)
{
	//int num = (int)data;
	
	for(;;) {
		pthread_mutex_lock(&mutex);
		if ((count & 1) == 0) {
			printf("even no = %d\n",count);
			pthread_cond_wait(&condVar,&mutex);
		}
		count++;
		pthread_cond_signal(&condVar);
		if (count > LOOP_LIMIT) {
			pthread_mutex_unlock(&mutex);
			return(NULL);
		}
		sleep(1);
		pthread_mutex_unlock(&mutex);
	}

}


void *oddFun(void *data)
{
	//int num = (int)data;
	
	for(;;) {
		pthread_mutex_lock(&mutex);
		if ((count & 1) != 0) {
			printf("odd no = %d\n",count);
			pthread_cond_wait(&condVar,&mutex);
		}
		count++;
		pthread_cond_signal(&condVar);
		if (count > LOOP_LIMIT) {
			pthread_mutex_unlock(&mutex);
			return(NULL);
		}
		sleep(1);
		pthread_mutex_unlock(&mutex);
	}

}

int main()
{
	pthread_t evenID, oddID;
	int count = 0;

	pthread_create(&evenID,NULL,evenFun,(void *)count);
	pthread_create(&oddID,NULL,oddFun,(void *)count);
	pthread_join(evenID,NULL);
	pthread_join(oddID,NULL);
	
	return 0;
}



