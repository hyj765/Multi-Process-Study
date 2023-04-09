#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>



pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


void* thread_function(void* arg){

  int id = (int)arg;

  if(pthread_mutex_lock(&mut) != 0){
	perror("mutex_lock");
	exit(-1);
  }
	// critical section start;	
	for(int i=0; i<10; ++i){
 	  printf("%d %d\n",i,id);
	}

  if(pthread_mutex_unlock(&mut) != 0){
	perror("mutex_unlock");
	exit(-1);
  }

  return "process Done";

}


int main(int argc, char* argv[]){

  pthread_t th1,th2;
  int id1 =1;
  int id2= 2;
  if(pthread_create(&th1,NULL,thread_function,(void*)id1) != 0){
	perror("pthread_create");
	return -1;
  }
  if(pthread_create(&th2,NULL,thread_function,(void*)id2) != 0){
	perror("pthread_create");
	return -1;
  }


  if(pthread_join(th1,NULL) !=0){
  	perror("pthread_join");
	return -1;
  }
  if(pthread_join(th2,NULL) !=0){
	perror("pthread_join");
	return -1;
  }


  if(pthread_mutex_destroy(&mut) !=0){
	perror("pthread_mutex_destroy");
	return -1;
  }

  return 0;

}
