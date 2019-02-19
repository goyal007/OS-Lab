#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
    int n=rand()%4;
    printf("childs  of root: %d",n);
    printf("\n");
    if(n>0)
    {
        int pid1;
        pid1=fork();
        n--;
        if(pid1==0) // first children
        {
            int temp=rand()%4;
            printf("No of child of 1st child of root: %d\n",temp);
            //printf("parent: %d\n",getppid());
            printf("1 child: %d parent:%d\n",getpid(),getppid());

            if(temp>0)
            {
                temp--;
                int p1=fork();
                if(p1==0)
                {
                	printf("1 child of 1 child %d parent: %d\n",getpid(),getppid());
                }
                else
                {
                    if(temp>0)
                    {
                        int p2=fork();
                        temp--;
                        if(p2==0)
                        {
                        	printf("2 child of 1 child  %d parent: %d",getpid(),getppid());
                        	printf("\n");

                        }
                        else
                        {
                            if(temp>0)
                            {
                            	int p3=fork();
                             	temp--;
                             	if(p3==0)
                             	{
                             		printf("3 child of 1 child %d parent: %d",getpid(),getppid());
                             		printf("\n");

                            	}
                            	else
                            	{
                                	sleep(1);

                            	}

                        	}
                        	else
                        	{
                                sleep(1);

                            }
                        }

                    }
                    else
                    {
                               sleep(1);

                     }
                }
            }
            else
            {
                sleep(1);

            }

        }

        else
        {
            sleep(2); //to wait for completion of 1st child
            //parent
            if(n>0)
            {
                int pid2=fork();
                n--;
                if(pid2==0)  // Second children
                {

                    int temp=rand()%4;
                    printf("No of child 2nd child of root: %d",temp);
                    printf("\n");
                    printf("2 child : %d parent %d:",getpid(),getppid());
                    printf("\n");

                    if(temp>0)
                    {
                        temp--;
                        int p1=fork();
                        if(p1==0)
                        {
                        	printf("1 child of 2 child %d parent: %d",getpid(),getppid());
                        	printf("\n");

                        }
                        else
                        {
                            if(temp>0)
                            {
                                temp--;
                                int p2=fork();
                                if(p2==0){
                                printf("2 child of 2 child  %d parent: %d",getpid(),getppid());
                                printf("\n");

                                }
                                else
                                {
                                    if(temp>0)
                                    {
                                        temp--;
                                        int p3=fork();
                                        if(p3==0)
                                        {
                                        printf("3 child of 2 child %d parent: %d",getpid(),getppid());
                                        printf("\n");

                                       }
                                        else
                                        {
                                            sleep(1);

                                        }
                                    }
                                    else
                                    {
                                            sleep(1);

                                     }
                                }
                             }
                             else
                             {
                                    sleep(1);

                             }
                         }
                    }
                    else
                    {
                          sleep(1);

                     }
                }

                else
                {
                     sleep(2);
                    if(n>0)
                    {
                        int pid3=fork(); //// third children
                        n--;
                        if(pid3==0)
                        {
                            //third child
                            int temp=rand()%4;
                            printf("No of child 3rd child of root: %d",temp);
							printf("\n");
                    		printf("3 child: %d parent %d:",getpid(),getppid());
							printf("\n");

                             if(temp>0)
                             {
                                 temp--;
                                int p1=fork();
                               if(p1==0)
                               {
                               printf("1 child of 3 child %d parent: %d",getpid(),getppid());
                               printf("\n");

                                  }
                               else
                               {
                                   if(temp>0)
                                   {
                                       temp--;
                                       int p2=fork();
                                       if(p2==0)
                                       {
                                       		printf("2 child of 3 child  %d parent: %d",getpid(),getppid());
                                       		printf("\n");

                                       }
                                       else
                                       {
                                            if(temp>0)
                                            {
                                                 temp--;
                                                int p3=fork();
                                                if(p3==0)
                                                {
                                                	printf("3 child of 3 child  %d parent: %d",getpid(),getppid());
                                                	printf("\n");

                                                 }
                                                else
                                                {
                                                    sleep(1);

                                                 }
                                              }
                                              else
                                              {
                                                sleep(1);

                                              }
                                          }
                                    }
                                    else
                                    {
                                            sleep(1);

                                     }
                                 }
                    		}
                    		else
                    		{
                          		sleep(1);

                       		 }

                        }
                        else
                        {

                            sleep(1);
                        }
                    }
                    else//parent..only 2 childs
                    {
                            sleep(1);
                    }
                }
            }
            else //parent..only 1 child
            {
                 sleep(1);
            }
        }
    }
    else //parent..no childs
    {
    	sleep(1);
    }
}
