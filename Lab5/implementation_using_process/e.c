#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h> 
//#include <sys/types.h> 
//#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/wait.h> 


#define	FIRST		1
#define	SECOND		2

int	shmID, 		// identifier for shared memory to store processNumber
	shrID, 		// identifier for shared memory to store shared value
	shmID2,
	shmID3;
key_t 	key1 = 54991, key2 = 784519,key3=34564,key4=87654;
int	 *p1_inside,*p2_inside, *shared,*favored; 
  

int main() 
{ 
	 srand(time(NULL));
    shmID = shmget(key1, sizeof(int), IPC_CREAT | 0660); // flag 
    if ( shmID < 0 ) 
    {
        printf("Shared memory 1 could not be allocated. Quitting.\n");
        exit(1); 
    } 

    shrID = shmget(key2, sizeof(int), IPC_CREAT | 0660); // flag 
    if ( shrID < 0 ) 
    {
        printf("Shared memory 2 could not be allocated. Quitting.\n");
        exit(1); 
    } 
    
    shmID2 = shmget(key3, sizeof(int), IPC_CREAT | 0660); // flag 
    if ( shmID2< 0 ) 
    {
        printf("Shared memory 3 could not be allocated. Quitting.\n");
        exit(1); 
    } 
    
    shmID3 = shmget(key4, sizeof(int), IPC_CREAT | 0660); // flag 
    if ( shmID3< 0 ) 
    {
        printf("Shared memory 3 could not be allocated. Quitting.\n");
        exit(1); 
    } 

    // Initialization is done in parent process
    	// shmat : attach shared memory segment. Returns (void *)-1 on failure else
    	// Attaches shared segment identified with shmID to an address 
    	// processNumber in process address space

    	p1_inside = (int *)shmat(shmID, NULL, 0); 
        if (p1_inside == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to first process. Quitting.\n");
            exit(-1); 
        } 
        
    	p2_inside = (int *)shmat(shmID2, NULL, 0); 
        if (p2_inside == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to first process. Quitting.\n");
            exit(-1); 
        } 
        
        favored = (int *)shmat(shmID3, NULL, 0); 
        if (favored == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to first process. Quitting.\n");
            exit(-1); 
        } 

    	shared = (int *)shmat(shrID, NULL, 0); 
        if (shared == (int *)-1 ) 
        {
            printf("Failed to attach shared variable to first process. Quitting.\n");
            exit(-1); 
        } 
    *shared = 0;
	*p1_inside = 0;
	*p2_inside = 0;
	*favored=1;
	//*processNumber = FIRST;
	

    // Create First (child) process
    if (fork() == 0) 
    { 
    	p1_inside = (int *)shmat(shmID, NULL, 0); 
        if (p1_inside == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to first process. Quitting.\n");
            exit(-1); 
        } 
        
    	p2_inside = (int *)shmat(shmID2, NULL, 0); 
        if (p2_inside == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to first process. Quitting.\n");
            exit(-1); 
        } 

		favored = (int *)shmat(shmID3, NULL, 0); 
        if (favored == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to first process. Quitting.\n");
            exit(-1); 
        } 		
		
    	shared = (int *)shmat(shrID, NULL, 0); 
        if (shared == (int *)-1 ) 
        {
            printf("Failed to attach shared variable to first process. Quitting.\n");
            exit(-1); 
        } 

	// This is the code first process executes
	int i,k;
 	for( i=0; i < 5; i++) 
 	{ 
		
		*p1_inside=1;
        while(*p2_inside)
       	{
       		if(*favored==2)
       		{
       			*p1_inside=0;
        		while(*favored==2);
        		*p1_inside=1;	
       			
       		}
        		
       	}
		
        	// Critical Section Begin 
		*shared = *shared+1;		

		printf("[%d] First: Critical Section (%2d).\n",i+1, *shared);
		*favored=2;
		*p1_inside=0;
		k = (int) ((5.0*rand())/RAND_MAX);
		sleep(k);
	}
    } 
    else 
    {
    // Create second (child) process
    if (fork() == 0) 
    {
     
    	p1_inside = (int *)shmat(shmID, NULL, 0); 
        if (p1_inside == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to 2 process. Quitting.\n");
            exit(-1); 
        } 
        
    	p2_inside = (int *)shmat(shmID2, NULL, 0); 
        if (p2_inside == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to 2 process. Quitting.\n");
            exit(-1); 
        } 
		
		favored = (int *)shmat(shmID3, NULL, 0); 
        if (favored == (int *)-1 ) 
        {
            printf("Failed to attach processNumber to 2 process. Quitting.\n");
            exit(-1); 
        } 		
		
    	shared = (int *)shmat(shrID, NULL, 0); 
        if (shared == (int *)-1 ) 
        {
            printf("Failed to attach shared variable to 2 process. Quitting.\n");
            exit(-1); 
        } 


	// This is the code second process executes
	int i,k;
 	for( i=0; i < 5; i++) 
 	{ 
 		*p2_inside=1;
        while(*p1_inside)
        {
        	if(*favored==1)
        	{
        		*p2_inside=0;
        		while(*favored==1);
        		*p2_inside=1;
        	}
        }	
		
        	// Critical Section Begin 
		*shared = *shared+1;		
		printf("[%d] Second: Critical Section (%2d).\n",i+1, *shared);
		*favored=1;
		*p2_inside=0;
		k = (int) ((5.0*rand())/RAND_MAX);
		sleep(k);
		//*processNumber = FIRST;
	}
    } 
    else 
    {
    	int i;
 	for( i=0; i < 5; i++) 
 	{
 	 
		*shared = *shared+12;		
		printf("[%d] Parent: Critical Section (%2d).\n",i+1, *shared);
	}
    	wait(NULL);
    	wait(NULL);
    }
    }
} 
