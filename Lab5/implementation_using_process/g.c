#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <limits.h>
int n=8;

int true=1;
int false=0;

int	shmID,shmID1, 		// identifier for shared memory to store choosing
	shrID; 		// identifier for shared memory to store shared value
key_t 	key1 = 5491, key2 = 784519, key3 = 887574;
int	*choosing, *ticket, *shared;

int maxValue()
{
    int m = -10000;
    int i;
    for(i=0;i<n;i++)
    {
        int val = *(ticket+i);
        if(val > m)
        {
            m = val;
        }
    }
    return m;
}

int main() 
{
    srand(time(NULL));
    int main_pid = getpid();
    shmID = shmget(key1, n*sizeof(int), IPC_CREAT | 0660); // flag
    if ( shmID < 0 ) 
    {
        printf("Shared memory 1 could not be allocated.\n");
        exit(1);
    }

    shmID1 = shmget(key3, n*sizeof(int), IPC_CREAT | 0660); // flag
    if ( shmID1 < 0 ) 
    {
        printf("Shared memory 2 could not be allocated.\n");
        exit(1);
    }

    shrID = shmget(key2, sizeof(int), IPC_CREAT | 0660); // flag
    if ( shrID < 0 ) 
    {
        printf("Shared memory 3 could not be allocated.\n");
        exit(1);
    }

	choosing = (int *)shmat(shmID, NULL, 0);
    if (choosing == (int *)-1 ) 
    {
        printf("Failed to attach choosing to main process.\n");
        exit(-1);
    }

    ticket = (int *)shmat(shmID1, NULL, 0);
    if (ticket == (int *)-1 ) 
    {
        printf("Failed to attach ticket to main process.\n");
        exit(-1);
    }

	shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1 ) 
    {
        printf("Failed to attach shared variable to main process.\n");
        exit(-1);
    }
	*shared = 0;
	int i;
    for(i=0;i<n;i++)
    {
        *(choosing+i) = false;
        *(ticket+i) = 0;
    }

    
    fork();
    fork();
    fork();
        
    choosing = (int *)shmat(shmID, NULL, 0);
    if (choosing == (int *)-1 ) 
    {
        printf("Failed to attach choosing to main process. Quitting.\n");
        exit(-1);
    }

    ticket = (int *)shmat(shmID1, NULL, 0);
    if (ticket == (int *)-1 ) 
    {
        printf("Failed to attach ticket to main process. Quitting.\n");
        exit(-1);
    }

    shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1 ) 
    {
        printf("Failed to attach shared variable to main process. Quitting.\n");
        exit(-1);
    }

    for(i=0;i<5;i++)
    {
        int pid = getpid();
        int x = pid-main_pid;
        *(choosing+x) = true;
        *(ticket+x) = maxValue()+1;
        *(choosing+x) = false;

        int j;
        for(j=0;j<n;j++)
        {
            if(j == x)
            {
                continue;
            }
            while(*(choosing+j)!=false);
            
            while(*(ticket+j)!=0 && *(ticket+j) < *(ticket+x));
            
            if(*(ticket+j) == *(ticket+x) && j < x)
            {
                while(*(ticket+j)!=0);
            }
        }

        *shared = *shared+1;

	printf("[%d] %d:In Critical Section (%2d).\n",i+1,x, *shared);
	*(ticket+x) = 0;

    }
    wait(NULL);
}
