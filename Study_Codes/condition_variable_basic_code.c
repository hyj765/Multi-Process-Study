#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condi = PTHREAD_COND_INITIALIZER;


volatile int ready = 0;
char buf[256];


void* producer(void* arg){
  printf("producer: ");
  fgets(buf, sizeof(buf),stdin);

  pthread_mutex_lock(&mut);
  ready= 1;

  if(pthread_cond_broadcast(&condi) != 0){
	perror("pthread_cond_broadcast");
	exit(-1);
  }

  pthread_mutex_unlock(&mut);
  return NULL;
}

void* consumer(void *arg){

  pthread_mutex_lock(&mut); // get mutex lock;
  while(!ready){
	
	if(pthread_cond_wait(&condi,&mut) != 0){
		perror("pthread_cond_wait");
		exit(-1);
	}
  }// if ready is false then return mutex to othere thread
  pthread_mutex_unlock(&mut);
  printf("consumer: %s\n", buf);

  return NULL;
}

int main(int argc,char * argv[]){

  pthread_t con,pro;
  pthread_create(&pro,NULL,producer,NULL);
  pthread_create(&con,NULL,consumer,NULL);

  pthread_mutex_destroy(&mut);

  pthread_join(pro,NULL);
  pthread_join(con,NULL);

  if(pthread_cond_destroy(&condi) != 0){
	perror("pthread_cond_destroy");
	return -1;
  }

  return 0;

}
