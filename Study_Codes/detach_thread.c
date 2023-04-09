#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


void* thread_func(void* arg){

	printf("detached_thread\n");
	int f = (int)arg;
	for(int i=0; i<f; ++i){
		printf("i = %d\n",i);
		sleep(1);
	}
	return NULL;
}


int main(int argc,char*argv[]){

	pthread_attr_t attr;
	if(pthread_attr_init(&attr) != 0) {
		perror("thread_attr_init_error");
		return -1;
	}

	if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED) != 0) {
		perror("thread_attr_setdetachstate");
		return -1;
	}


	pthread_t th;
	int f = 7;
	if(pthread_create(&th, &attr, thread_func, (void*)f) != 0){
		perror("thread_create_error");
		return -1;
	}
	
	if(pthread_attr_destroy(&attr) != 0){
		perror("pthread_attr_destoy_error");
		return -1;
	}
	printf("main_thread\n");

	sleep(10);
	return 0;

}
