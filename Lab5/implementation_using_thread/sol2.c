// gcc -pthread <name.c>
// This program creates two threads, each thread 

/* Incorrect solution as does not even guarantee mutual exclusion problem. P1 checks P2_inside and
 finding it false exits the inner while loop. But before P1 could set P1_inside=true; context switch takes place.
  P2 gets the chance and finding P2_inside=false, exits loop and is ready for entry to critical section.
   So both processes can enter critical section ----violating mutual exclusion property-----.*/

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
  
int global_arr[10]={0,1,2,3,4,5,6,7,8,9};
//int 	threadNumber;
  
void Initialization() { 
	//threadNumber = THREAD1;
	srand(time(NULL));
} 

void	*func1(void *s) { 
	int	i,k;
	int z=0;
    for (i=0; i < 5; i++)  
    {
		//	sleep(5);
		while(p2_inside) ;	// Wait
		
		sleep(2); ////context switch here for showing no mutual exclusion
		
		p1_inside=1;
		// Critical Section - shared variable is incremented
	
		for(z=0;z<10;z++)
		{
			printf("process 1: array index:%d array val: %d\n",z,global_arr[z]);
		}
		
		
		
		x = x+1;
		printf("[%2d] :  Thread 1 in Critical Section (%d).\n", i+1,x);
		p1_inside=0;
		// Random delay added
		k = (int) ((3.0*rand())/RAND_MAX);
		sleep(k);
	}
} 
  
  
void	*func2(void *s) { 
	int	i, k,z;
    for (i=0; i < 5; i++)   	
    {
		//sleep(2);
		while(p1_inside) ;	// Wait
	
		p2_inside=1;
		// Critical Section - shared variable is incremented

		for(z=0;z<10;z++)
		{
			printf("process 2: array index:%d array val: %d\n",z,global_arr[z]);
		
			sleep(1); //context switch here for showing no mutual exclusion
		}	
		
		x = x+1;
		printf("[%2d] :  Thread 2 in Critical Section (%d).\n", i+1,x);
		p2_inside=0;
		// Random delay added
		int k;
		k = (int) ((5.0*rand())/RAND_MAX);
		sleep(k);
	}
} 
  

int main() { 
    	pthread_t 	Thread1, Thread2; 
    	// Initialized the lock then fork 2 threads 
		Initialization();
  
    	// Create two threads (both run func)  
    	pthread_create(&Thread1, NULL, func1, (void *) THREAD1); 
    	pthread_create(&Thread2, NULL, func2, (void *) THREAD2); 
  
	// Wait for the threads to end. 
    	pthread_join(Thread1, NULL); 
    	pthread_join(Thread2, NULL); 
  
    	return 0; 
} 
