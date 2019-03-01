#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <limits.h>
int	x = 0;		// Shared variable
int ticket[10];/* Value of the ticket for each process initialized to 0 */
int choosing[10];/* Array to record which processes are taking a ticket */
int max()
{
	int max=-10000;
	int i=0;
	for(i=0;i<10;i++)
	{
		if(ticket[i]>max)
		max=ticket[i];
	}
	return max;
}

void	*func1(void *s) 
{ 
	
	int process=(int)s;
	int j;
	for( j=0;j<10;j++)
	{
		choosing[process]=1;
		ticket[process]=max()+1;
		choosing[process]=0;
		int i;
		/* Wait until turn of current ticket comes */
		for(i=0;i<10;i++)
		{
			if(i==process)
				continue;
			while(choosing[i]!=0);/* Busy wait while Ti is choosing a ticket */
			
			
			/* Busy wait while current ticket value is lowest */
			while(ticket[i]!=0 && ticket[process]>ticket[i]);
			
			/* In case of tie, favor smaller process number */
			if(ticket[process]==ticket[i]  && i<process)
			{
				while(ticket[i]!=0);
			}
		}
		//critical sec
		x = x+1;
		printf("[%2d] : in Critical Section (%d).\n", process,x);
		ticket[process]=0; //its work is done
		int k;
		k = (int) ((3.0*rand())/RAND_MAX);
		sleep(k);
	}

	
} 
  


int main() 
{ 
	srand(time(NULL));
    pthread_t 	thread[10];
    int i;
	for(i=0;i<10;i++)
		pthread_create(&thread[i], NULL, func1, (void *)i); 	
	
  	for(i=0;i<10;i++)
		pthread_join(thread[i], NULL); 	
	

    return 0; 
}
