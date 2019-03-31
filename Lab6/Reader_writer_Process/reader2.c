#include<stdio.h>
#include<stdlib.h>   /* exit(), malloc(), free() */
#include<unistd.h>
#include<time.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/shm.h>  /* shmat(), IPC_RMID        */
#include<sys/types.h>  /* key_t, sem_t, pid_t      */
#include<fcntl.h>       /* O_CREAT, O_EXEC          */

int	shmID,shmID1,shmID2;  //readcount,shared,writecount variable
int	*readcount,*shared,*writecount;
sem_t  *rmutex, *wmutex, *readtry,*resource;
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
    shmID2=shmget(key3,sizeof(int),IPC_CREAT | 0660);
    if(shmID2 <0)
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

    writecount = (int *)shmat(shmID2, NULL, 0);
    if (writecount == (int *)-1 )
    {
        printf("Failed to attach shared to main process. Quitting.\n");
        exit(-1);
    }


    rmutex=sem_open("read_mutex",O_CREAT |O_EXCL,0644,1);
    wmutex = sem_open("write_mutex",O_CREAT | O_EXCL,0644, 1);
    readtry = sem_open("readtry",O_CREAT | O_EXCL,0644, 1);
    resource=sem_open("resource",O_CREAT |O_EXCL,0644,1);

    sem_unlink("read_mutex");
    sem_unlink("write_mutex");
    sem_unlink("readtry");
    sem_unlink("resource");

    *readcount = 0;
    *writecount = 0;
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

        writecount = (int *)shmat(shmID2, NULL, 0);
        if (writecount == (int *)-1 )
        {
            printf("Failed to attach shared to writer process. Quitting.\n");
            exit(-1);
        }

	    //writer code
	    sem_wait(wmutex);
        *writecount++;
        if(*writecount == 1)
        {
            sem_wait(readtry);
        }
        sem_post(wmutex);

    	sem_wait(resource);
	    *shared+=2;
	    printf("value written  by writer%d : %d\n",getpid(),*shared);
	   // sleep(1);// to see writer preference
	    sem_post(resource);
	    sem_wait(wmutex);
        *writecount--;
        if(*writecount == 0)
        {
            sem_post(readtry);
        }
        sem_post(wmutex);
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

            writecount = (int *)shmat(shmID2, NULL, 0);
            if (writecount == (int *)-1 )
            {
                printf("Failed to attach shared to reader process. Quitting.\n");
                exit(-1);
            }
	        //reader code

	        sem_wait(readtry);
        	sem_wait(rmutex);
    	    *readcount++;
    	    if(*readcount == 1)
    	    {
        		sem_wait(resource);
    	    }
    	    sem_post(rmutex);
            sem_post(readtry);
	        printf("value read by reader%d : %d\n",getpid(),*shared);
	        // sleep(1);// to see writer preference
	        sem_wait(rmutex);
    	    *readcount--;
    	    if(*readcount == 0)
    	    {
    		    sem_post(resource);
    	    }
    	    sem_post(rmutex);
            sleep(rand() % 4);


        }
        else
        {
            wait(NULL);
            wait(NULL);

            shmdt(resource);
            shmctl(shmID,IPC_RMID, 0);
    	    shmdt(rmutex);
    	    shmctl(shmID1,IPC_RMID, 0);
    	    shmdt(readtry);
            shmctl(shmID2,IPC_RMID, 0);
            shmdt(wmutex);



    	    /* cleanup semaphores */

            sem_destroy (rmutex);
            sem_destroy (resource);
            sem_destroy (readtry);
            sem_destroy (wmutex);
        }
    }
}