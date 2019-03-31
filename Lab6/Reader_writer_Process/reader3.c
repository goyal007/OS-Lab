#include<stdio.h>
#include<stdlib.h>   /* exit(), malloc(), free() */
#include<unistd.h>
#include<time.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/shm.h>  /* shmat(), IPC_RMID        */
#include<sys/types.h>  /* key_t, sem_t, pid_t      */
#include<fcntl.h>       /* O_CREAT, O_EXEC          */

int	shmID,shmID1;  //readcount,shared variable
int	*readcount,*shared;
 sem_t *resourceAccess, *readcountAccess, *serviceQueue;
key_t key1 = 5491, key2 = 784519, key3 = 789456;
int main()
{
    srand(time(0));

    shmID=shmget(key1,sizeof(int),IPC_CREAT | 0660);
    if(shmID <0)
    {
        printf("Shared memory could not be allocated. Quitting.\n");
        exit(1);
    }
    shmID1=shmget(key2,sizeof(int),IPC_CREAT | 0660);
    if(shmID1 <0)
    {
        printf("Shared memory could not be allocated. Quitting.\n");
        exit(1);
    }


    readcount = (int *)shmat(shmID, NULL, 0);
    if (readcount == (int *)-1 )
    {
        printf("Failed to attach readcount to main process. Quitting.\n");
        exit(-1);
    }

    shared = (int *)shmat(shmID1, NULL, 0);
    if (shared == (int *)-1 )
    {
        printf("Failed to attach shared to main process. Quitting.\n");
        exit(-1);
    }




    resourceAccess=sem_open("resourceAccess",O_CREAT |O_EXCL,0644,1);
    readcountAccess=sem_open("readcountAccess",O_CREAT |O_EXCL,0644,1);
    serviceQueue = sem_open("serviceQueue",O_CREAT | O_EXCL,0644, 1);

    sem_unlink("resourceAccess");
    sem_unlink("serviceQueue");
    sem_unlink("readcountAccess");

    *readcount = 0;
	*shared = 1;

    if(fork()==0)  //writer
    {
        fork();
    	fork();
    	fork();

    	readcount = (int *)shmat(shmID, NULL, 0);
	    if (readcount == (int *)-1 )
	    {
	        printf("Failed to attach readcount to writer process. Quitting.\n");
	        exit(-1);
	    }

	    shared = (int *)shmat(shmID1, NULL, 0);
	    if (shared == (int *)-1 )
	    {
	        printf("Failed to attach shared to writer process. Quitting.\n");
	        exit(-1);
	    }



	    //writer code
	    sem_wait(serviceQueue);
        sem_wait(resourceAccess);
        sem_post(serviceQueue);
	    *shared+=2;
	    printf("value written  by writer%d : %d\n",getpid(),*shared);
	   // sleep(1);
	    sem_post(resourceAccess);
	    sleep(rand() % 4);
    }
    else
    {
        if(fork()==0)  //reader
        {
            fork();
        	fork();
        	fork();

    	    readcount = (int *)shmat(shmID, NULL, 0);
	        if (readcount == (int *)-1 )
	        {
    	        printf("Failed to attach readcount to reader process. Quitting.\n");
	            exit(-1);
	        }

	        shared = (int *)shmat(shmID1, NULL, 0);
	        if (shared == (int *)-1 )
	        {
	            printf("Failed to attach shared to reader process. Quitting.\n");
	            exit(-1);
	        }


	        //reader code

	        sem_wait(serviceQueue);
            sem_wait(readcountAccess);
             if(*readcount == 0)
             {
                sem_wait(resourceAccess);
            }
            *readcount++;
            sem_post(serviceQueue);
            sem_post(readcountAccess);
	        printf("value read by reader%d : %d\n",getpid(),*shared);
	       // sleep(1);
	        sem_wait(readcountAccess);
            *readcount--;
            if(*readcount == 0)
            {
                sem_post(resourceAccess);
            }
            sem_post(readcountAccess);
            sleep(rand() % 4);


        }
        else
        {
            wait(NULL);
            wait(NULL);
            //detach
            shmdt(resourceAccess);
            shmctl(shmID,IPC_RMID, 0);
    	    shmdt(readcountAccess);
    	    shmctl(shmID1,IPC_RMID, 0);
    	    shmdt(serviceQueue);
    	    /* cleanup semaphores */

            sem_destroy (resourceAccess);
            sem_destroy (readcountAccess);
            sem_destroy (serviceQueue);
        }
    }
}