#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <sys/types.h>
char* queue[1000];
char* extra = "---";
char* indentation;
int front=0,rear=-1;

void bfs()
{
    indentation = (char*)malloc(500*sizeof(char));
    strcpy(indentation,"|---");
  while(front < rear)
  {
  		char* path = queue[front];
  		front++;
  		//insert null after every level in bfs
  		if(path == NULL)
  		{
  		    queue[++rear] = NULL;
  		    path = queue[front];
  		    front++;
  		    strcat(indentation,extra);
  		}
	  DIR * d = opendir(path);
	  if(d==NULL)
		return;
	  struct dirent * dir;

	  while ((dir = readdir(d)) != NULL)
		{

		  if(dir->d_type != DT_DIR)
		  {
		    printf("%s%s\n",indentation, dir->d_name);
		  }
		  else if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 )
		  {
		    printf("%s%s\n",indentation, dir->d_name);
		    char* dir_path = (char*)malloc(255*sizeof(char));

			char * te="/\0";
		    strcpy(dir_path,path);
		    strcat(dir_path,te);
		    strcat(dir_path,dir->d_name);
		    //sprintf(dir_path, "%s/%s", path, dir->d_name);
		    queue[++rear] = dir_path;
		  }
		}
		closedir(d);
	}
}
int main(int argc, char **argv)
{
	printf("%s\n",argv[1]);
	queue[++rear] = argv[1];
	queue[++rear] = NULL;
    bfs();
  return(0);
}
