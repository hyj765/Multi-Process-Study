#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<pthread.h>

int cnt =0; // threads access this value;

void* thread_function(void* arg){

  int id = (int)arg;
  sem_t *s = sem_open("/samplesemaphore",0);
  if(s == SEM_FAILED){
	perror("sem_open");
	exit(1);
  }

  for(int i=0; i<10; ++i){
	if(sem_wait(s)== -1 ){ // wait for get a lock
		perror("sem_wait");
		exit(1);
	}

	__sync_fetch_and_add(&cnt,1);
	//atomic increase function
	printf("get in id=%d cnt=%d\n",id,cnt);
	usleep(10000); 
	
	__sync_fetch_and_sub(&cnt,1);
	printf("get out id=%d cnt=%d\n",id,cnt);
	//atomic decrease function
	if(sem_post(s) == -1){
		perror("sem_post");
		exit(1);
	}// escape ciritical section
  }
	
  if(sem_close(s) == -1){
	perror("sem_close");
  }

  return NULL;
}

int main(int argc, char* argv[]){

  sem_t *s = sem_open("/samplesemaphore",O_CREAT,0660,3);
  //name, create if not exist, authority, max thread
  
  if(s == SEM_FAILED){
	perror("sem_open");
	return 1;
  }

  pthread_t v[10];
  for(int i=0; i<10; ++i){
	pthread_create(&v[i],NULL,thread_function,(void*)i);
  }


  for(int i=0; i<10; ++i){
	pthread_join(v[i],NULL);
  }

  if(sem_close(s)==-1){
	perror("sem_close");
  }

  if(sem_unlink("/samplesemaphore") == -1){
	perror("sem_unlink");
  }

  return 0;
}
