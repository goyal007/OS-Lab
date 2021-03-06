#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
#include<pthread.h>
#define n1 10
#define n2 10
int read_count,shared_data;
sem_t resource,rmutex;
void * read(void * args)
{
	int r_id;
  	r_id= (int)args;
	sem_wait(&rmutex);
	read_count++;
	if(read_count==1)
	{
		sem_wait(&resource);
	}
	sem_post(&rmutex);

	printf("value read by reader%d : %d\n",r_id,shared_data);
	sleep(1);   // to see reader preference

	sem_wait(&rmutex);
	read_count--;
	if(read_count==0)
	{
		sem_post(&resource);
	}

	sem_post(&rmutex);
	sleep(rand()%4);
}
void * write(void * args)
{
	int w_id;
  	w_id= (int)args;
	sem_wait(&resource);

	shared_data=shared_data+2;
	printf("value written  by writer%d : %d\n",w_id,shared_data);
	sleep(1);// to see reader preference
	sem_post(&resource);
	sleep(rand()%4);

}
int main()
{
	int i,j;
	pthread_t r[n1],w[n2];

	sem_init(&rmutex,0,1);
	sem_init(&resource,0,1);
	for(i=0;i<n2;i++)
	{
		pthread_create(&w[i],NULL,&write,(void *)i+1);
		pthread_create(&r[i],NULL,&read,(void *)i+1);
	}
//	for(i=0;i<n1;i++)
//		pthread_create(&r[i],NULL,&read,(void *)i+1);
	for(i=0;i<n2;i++)
	{
		pthread_join(w[i],NULL);
		pthread_join(r[i],NULL);
	}
//	for(i=0;i<n1;i++)
//		pthread_join(r[i],NULL);

	return 0;
}
