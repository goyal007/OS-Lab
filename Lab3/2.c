#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/types.h>
int main()
{
	FILE *fp=fopen("input.txt","r");
	char buffer[100],buffer1[100],temp1[100],temp2[100],temp3[100];
	int pipe1[2],pipe2[2],pipe3[2];
	if(pipe(pipe1)==-1 || pipe(pipe2)==-1 || pipe(pipe3)==-1)
	{
		perror("pipe Fail\n");
	    exit(-1);
	}
	fgets(buffer,100,fp);
	int pi=fork();
	if(pi < 0)
	{
		printf("Fork Failed\n");
		exit(-1);
	}
	else if(pi>0) //1
	{
		printf("Process:1\n");
		int p;
		int l1;
		l1=strlen(buffer);
		printf("In Process1%s\n",buffer);
		write(pipe1[1],buffer,l1+1);
		wait(NULL);
		printf("Process:1\n");
		//printf("2\n");
		int count_word=0;
		read(pipe3[0],&count_word,sizeof(int));
		printf("Total word=%d\n",count_word);
	}
	else if(pi==0)
	{
		printf("Process:2\n");
		read(pipe1[0],buffer1,100);
		//printf("1234::%s\n",buffer1);
		//printf("3\n");
		int l2;
		l2=strlen(buffer1);
		int j=0,i=0;
		while(i<l2)
		{
			while((buffer1[i]==' ' || buffer1[i]=='\t')  && i<l2)  //remove starting spaces
			{
				i++;
				continue;
			}
			while((buffer1[i]!=' ' && buffer1[i]!='\t')   && i<l2)
			{
				temp1[j]=buffer[i];
				j++;
				i++;
			}
			temp1[j]=' ';  ////add space in new array
			j++;
		}
		temp1[j]='\0';
		int l3=j;
		j=0;
		i=0;
		while(i<l3)
		{
			if(ispunct(temp1[i]))  //append space after punctuation mark
			{
				temp2[j]=temp1[i];
				j++;
				temp2[j]=' ';
				j++;
			}
			else if(temp1[i]==' ' && (i+1<l3) && ispunct(temp1[i+1]) )  //remove space before punctuation mark
			{
				temp2[j]=temp1[i+1];
				j++;
				temp2[j]=' ';
				j++;
				i++;
			}
			else
			{
				temp2[j]=temp1[i];
				j++;
			}
			i++;
		}
		temp2[j]='\0';
		printf("In Process2:%s\n",temp2);
		int l4;
		l4=strlen(temp2);
		write(pipe2[1],temp2,l4+1);
		int pi2;
		pi2=fork();
		//printf("3a\n");
		if(pi2 < 0)
		{
			printf("Fork Failed\n");
			exit(-1);
		}
		else if(pi2==0)
		{
			printf("Process:3\n");
			//printf("4\n");
			int count_word=0;
			read(pipe2[0],temp3,100);
			//printf("1234::%s\n",temp3);
			int l5;
			l5=strlen(temp3);
			int i=0,j=0;
			while(i<l5)
			{
			//	temp3[i]=toupper(temp3[i]);
				printf("%c",toupper(temp3[i]));
				i++;
				while(i<l5 && temp3[i]!=' ')
				{
					//temp3[i]=tolower(temp3[i]);
					printf("%c",tolower(temp3[i]));
					i++;
				}
				printf(" ");
				i++;
				count_word++;
			}
			write(pipe3[1],&count_word,sizeof(int));
			//printf("1234:Total word=%d\n",count_word);
		}
		else if(pi2>0) //1
		{
			wait(NULL);
		}
	}
	return 0;
}
