#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int value = 0;

void* reader(void *arg){
 
 if(pthread_rwlock_rdlock(&rwlock) != 0){
	perror("rwlock");
	exit(-1);
 }
 
 printf("%d\n ",value);

 if(pthread_rwlock_unlock(&rwlock) != 0){
	perror("unlock_read");
	exit(-1);
 }

 return NULL;
}

void* writer(void*arg){

  if(pthread_rwlock_wrlock(&rwlock) !=0){
	perror("write_lock");
	exit(-1);
  }
  for(int i=0; i<10; ++i){
	value++;
  }
  if(pthread_rwlock_unlock(&rwlock) != 0){
	perror("write_lock");
	exit(-1);
  }
  return NULL;
}


int main(int argc,char* argv[]){

  pthread_t wrthread[3];
  pthread_t rthread[3];

  for(int i=0; i<3; ++i){
	if(pthread_create(&wrthread[i],NULL,writer,NULL) !=0){
		perror("thread_create _writer");
		return -1;
	}
	if(pthread_create(&rthread[i],NULL,reader,NULL) != 0){
		perror("thread_create _reader");
		return -1;
	}
  }

  for(int i=0; i<3; ++i){
	pthread_join(wrthread[i],NULL);
	pthread_join(rthread[i],NULL);
  }

  if(pthread_rwlock_destroy(&rwlock) !=0){
	perror("pthread_rwlock_destroy");
	return -1;
  }

  printf("%d",value);

  return 0;
}
