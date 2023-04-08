#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


void* thread_func(void *arg){

    int id = (int)arg;
    for(int i=0; i<5; ++i){

	printf("id = %d, i = %d\n", id,i);
   	sleep(1);
    }

    	return "finished!";
}


int main(){

  pthread_t v[10];
  for(int i=0; i<10; ++i){
	if(pthread_create(&v[i],NULL,thread_func,(void*)i) != 0) {
		perror("pthread_create");
		return -1;
	}


  }


  for(int i=0; i<10; ++i){
	char* ptr;
	if(pthread_join(v[i], (void**)&ptr) == 0){
		printf("msg = %s\n", ptr);
	}else{	
		perror("pthread_join");
		return -1;
	}
  }


  return 0;
}
