#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>



pthread_mutex_t barrier_mt = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;
volatile int num = 0;

void barrier(volatile int *cnt, int max){
  if(pthread_mutex_lock(&barrier_mt) != 0){
	perror("pthread_mutex_lock");
	exit(-1);
  }

  (*cnt)++;

  if(*cnt == max) {

	if(pthread_cond_broadcast(&barrier_cond) !=0){
		perror("pthread_cond_broadcast");
		exit(-1);
	}

  }else{
	
	  do{
	  	if(pthread_cond_wait(&barrier_cond,&barrier_mt) != 0){
			perror("pthread_cond_wait");
			exit(-1);
		  }
	  }while(*cnt < max);
  }

  if(pthread_mutex_unlock(&barrier_mt) != 0){
	perror("pthread_cond_wait");
	exit(-1);
  }

}

void *worker(void *arg){
  barrier(&num,10);
  int id = (int)arg;

  for(int i=0; i<10; ++i){
	printf("id %d: %d\n", id, i);

  }

  return NULL;
}


int main(int argc,char* argv[]){


  pthread_t threads[10];
  for(int i=0; i<10; ++i){
	if(pthread_create(&threads[i],NULL,worker,(void*)i)!=0){
		perror("thread_create");
		exit(-1);
	}
  }

  for(int i=0; i<10; ++i){
	pthread_join(threads[i],NULL);
  }

  pthread_mutex_destroy(&barrier_mt);
  if(pthread_cond_destroy(&barrier_cond) != 0){
	perror("pthread_cond_destory");
	return -1;
  }
}
