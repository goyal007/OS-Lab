// NP- 1C

#include<stdio.h>
#include<stdlib.h>   /* exit(), malloc(), free() */
#include<unistd.h>
#include<time.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/shm.h>  /* shmat(), IPC_RMID        */
#include<sys/types.h>  /* key_t, sem_t, pid_t      */
#include<fcntl.h>       /* O_CREAT, O_EXEC          */

# define n 20
int shmID,shmID1,// identifier for shared memory to store processNumber
    shmID2;// identifier for shared memory to store shared value

key_t key1 = 5491, key2 = 784519, key3 = 789456;
int *queue,*tail,*head;

int main()
{
    srand(time(0));

    sem_t *nempty,*nfull,*sem_lock;//num of empty space and number of filled

    shmID=shmget(key1,sizeof(int)*n,IPC_CREAT | 0660);
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

    //link and initialize the shared mem  in main()

    queue=(int *)shmat(shmID,NULL,0);
    if (queue == (int *)-1 )
    {
        printf("Failed to attach queue to main process. Quitting.\n");
        exit(-1);
    }
    head=(int *)shmat(shmID1,NULL,0);
    if (head == (int *)-1 )
    {
        printf("Failed to attach queue to main process. Quitting.\n");
        exit(-1);
    }
    tail=(int *)shmat(shmID2,NULL,0);
    if (tail == (int *)-1 )
    {
        printf("Failed to attach queue to main process. Quitting.\n");
        exit(-1);
    }


    //initialize semaphore

    nempty=sem_open("empty_space",O_CREAT | O_EXCL,0664,n);
    nfull=sem_open("filled_space",O_CREAT | O_EXCL,0664,0);
    sem_lock=sem_open("binary_lock",O_CREAT | O_EXCL,0664,1);
    /* unlink prevents the semaphore existing forever */
    sem_unlink("empty_space");
    sem_unlink("filled_space");
    sem_unlink("binary_lock");

    //initialize var of shared mem
    *head=0;
    *tail=0;

    //  First child process (producer)
    if(fork()==0)
    {
        //create multiple producer
        fork();
        fork();
        fork();
        //link the shared mem  for multiple producers

        queue=(int *)shmat(shmID,NULL,0);
         if (queue == (int *)-1 )
        {
            printf("Failed to attach queue to producer process. Quitting.\n");
            exit(-1);
        }
        head=(int *)shmat(shmID1,NULL,0);
        if (head == (int *)-1 )
        {
            printf("Failed to attach queue to producer process. Quitting.\n");
            exit(-1);
        }
        tail=(int *)shmat(shmID2,NULL,0);
        if (head == (int *)-1 )
        {
            printf("Failed to attach queue to producer process. Quitting.\n");
            exit(-1);
        }

        //code for producer

        while(1)
        {
            sem_wait(nempty);/* wait for empty place in queue */
            sem_wait(sem_lock);
            int item;
            item=rand()%100;

		    printf("produced product number: %d by %d\n",item,getpid());
		    queue[*tail]=item;
		    *tail=(*tail+1)%n;
		    sem_post(sem_lock);
		    sem_post(nfull);  /* tells there are empty in queue*/
		    sleep(rand()%4); /* rest after product produce */
        }



    }
    else
    {
        //  sec child process (consumer)
        if(fork()==0)
        {
            //link the shared mem  in consumer
            queue=(int *)shmat(shmID,NULL,0);
            if (queue == (int *)-1 )
            {
                printf("Failed to attach queue to consumer process. Quitting.\n");
                exit(-1);
            }
            head=(int *)shmat(shmID1,NULL,0);
            if (head == (int *)-1 )
            {
                printf("Failed to attach queue to consumer process. Quitting.\n");
                exit(-1);
            }
            tail=(int *)shmat(shmID2,NULL,0);
            if (head == (int *)-1 )
            {
                printf("Failed to attach queue to consumer process. Quitting.\n");
                exit(-1);
            }


            //code for consumer

            while(1)
            {
                int item;

                sem_wait(nfull);/* wait for products in queue */
		        item=queue[*head];
		        *head=(*head+1) % n;
		        printf("\t\t\t\tconsumed product number: %d\n", item);
		        sem_post(nempty);/* tells inc in  free spaces in the queue*/
		        sleep(rand()%4);/* consume the product */
            }
        }
        else
        {
            //two wait for 2 child process
            wait(NULL);
            wait(NULL);

            /* shared memory detach */

            shmdt(queue);
            shmctl(shmID,IPC_RMID,0);
            shmdt(nempty);
            shmctl(shmID1,IPC_RMID,0);
            shmdt(nfull);
            shmctl(shmID2,IPC_RMID,0);

            /* cleanup semaphores */

            sem_destroy (nempty);
            sem_destroy (nfull);
        }
    }

}