#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
sem_t sema[5];
sem_t lock;
int left(int pid)
{
	return pid;
}
int right(int pid)
{
	return (pid+1)%5;
}
void * func(void * args)
{
	int pid=(int)args;
	while(1)
	{
		//sleep(1);
		printf("Thinking!!! -> %d Philospher\n",pid);
		sleep(1);
		//sem_wait(&lock);
		if(pid==4)
		{
			sem_wait(&sema[right(pid)]);
			printf("Grabbed %d fork by %d Philospher\n",right(pid),pid);
			sem_wait(&sema[left(pid)]);
			printf("Grabbed %d fork by %d Philospher\n",left(pid),pid);
			sleep(1);
			printf("Eating start by %d Philospher\n",pid);
			//printf("eating!!! -> %d\t\t grabbed forks are %d and %d\n",pid,left(pid),right(pid));
			
			sleep(1);
		}
		else
		{
			sem_wait(&sema[left(pid)]);
			printf("Grabbed %d fork by %d Philospher\n",left(pid),pid);
			sem_wait(&sema[right(pid)]);
			printf("Grabbed %d fork by %d Philospher\n",right(pid),pid);
			sleep(1);
			printf("Eating start by %d Philospher\n",pid);
			//printf("eating!!! -> %d\t\t grabbed forks are %d and %d\n",pid,left(pid),right(pid));
			
			sleep(1);
		}
		//sem_post(&lock);
		sleep(1);
		
		sem_post(&sema[left(pid)]);
		printf("Leaved %d fork by %d Philospher\n" ,left(pid),pid);
		sem_post(&sema[right(pid)]);
		printf("Leaved %d fork by %d Philospher\n" ,right(pid),pid);
		//printf("leaved forks are %d and %d by %d philospher\n" ,left(pid),right(pid),pid);
		//sleep(1);
	}
}
int main()
{
	pthread_t philospher[5];
	int i,j;
	sem_init(&lock,0,1);
	for(i=0;i<5;i++)
	{
		sem_init(&sema[i],0,1);
	}
	
	for(i=0;i<5;i++)
	{
		pthread_create(&philospher[i],NULL,func,(void *)i);
	}
	
	for(i=0;i<5;i++)
	{
		pthread_join(philospher[i],NULL);
	}
	return 0;
}
