#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
int main()
{

	char command[100],str1[100];
	printf("-> ");
	fgets(command,100,stdin);
	while(1)
	{
		if(!(strcmp(command,"exit\n")))
		{
			printf("Terminated\n");
			break;
		}

		int pi=fork();
	//	printf("%d   %d\n",pi,getpid());
		if(pi<0)
		{
			printf("Fork Failed\n");
			break;
		}
		else if(pi==0)
		{
		   // printf("%d child  %d\n",pi,getpid());
			int count=0;
			int l=strlen(command);
			char* args[10];
			int i=0;
			while(i<l)
			{
				int j=0;
				while(command[i]!=' ' && command[i]!='\0'&& command[i]!='\n')
				{
					str1[j]=command[i];
					j++;
					i++;
				}
				i++;
				str1[j]='\0';
				args[count]=strdup(str1);
				count++;
			}
			args[count]=NULL;
			execvp(args[0],args);
		}
		else if(pi>0)
		{
		   // printf("%d parent00  %d\n",pi,getpid());
			wait(NULL);
			//printf("%d  parent %d\n",pi,getpid());
			printf("-> ");
			fgets(command,100,stdin);
		}

	}
	return 0;
}
