// gcc -pthread <name.c>
// This program creates two threads, each thread 

/* Deadlock can take place. P1 executes first statement and sets
P1_wantsToEnter=true. Context switch happens. And now P2 runs and sets
P2_WantsToEnter=true. Now both processes are deadlocked. */
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

int	x = 0;		// Shared variable
int p1_inside,p2_inside;

#define	THREAD1		0
#define	THREAD2		1

#define	TRUE		1
#define FALSE		0

void	*func1(void *s) 
{ 
	int	i, k;
    for (i=0; i < 5; i++)  
    {
    	p1_inside=1;
		//sleep(2);  //for context switch for showing deadlock do sleep only 1 process
		while(p2_inside) ;	// Wait
		
		// Critical Section - shared variable is incremented
		x = x+1;
		printf("[%2d] :  Thread 1 in Critical Section (%d).\n", i+1,x);
		p1_inside=0;
		
		// Random delay added
		k = (int) ((3.0*rand())/RAND_MAX);
		sleep(k);
	}
} 
  
  
void	*func2(void *s) 
{ 
	int	i, k;
    	
    for (i=0; i < 5; i++)  
    {
    	p2_inside=1;
	
		while(p1_inside) ;	// Wait
		
		// Critical Section - shared variable is incremented
		x = x+1;
		printf("[%2d] :  Thread 2 in Critical Section (%d).\n", i+1,x);
		p2_inside=0;
		// Random delay added
		//int k;
		k = (int) ((5.0*rand())/RAND_MAX);
		sleep(k);
	}
} 
  

int main() 
{ 
    	pthread_t 	Thread1, Thread2; 
    	// Initialized the lock then fork 2 threads 
    	
		//	Initialization();
		//srand(time(NULL));
  		//printf("In main\n");
    	// Create two threads (both run func)  
    	pthread_create(&Thread1, NULL, func1, (void *) THREAD1); 
    	pthread_create(&Thread2, NULL, func2, (void *) THREAD2); 
  
	// Wait for the threads to end. 
    	pthread_join(Thread1, NULL); 
    	pthread_join(Thread2, NULL); 
  
    	return 0; 
} 
