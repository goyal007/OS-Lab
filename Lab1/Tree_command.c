#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include<stdlib.h>

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
		    queue[++rear] = dir_path;
		  }
		}
		closedir(d);
	}
}
void dfs(char *basePath, const int root);


int main(int argc, char **argv)
{
    char path[100];
    printf("Enter path: ");
    scanf("%s", path);
    if(strcmp(argv[1],"-b")==0)
    {
        printf("%s\n",path);
    	queue[++rear] = path;
	    queue[++rear] = NULL;
        bfs();
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        dfs(path, 0);
    }
  return(0);
}


void dfs(char *basePath, const int root)
{
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
	{
	//printf("wrong directory");
	return;
	}


    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            for (i=0; i<root; i++)
            {
                if (i%2 == 0 || i == 0)
                    printf("|");
                else
                    printf(" ");
            }

            printf("|--%s\n", dp->d_name);
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            dfs(path, root + 2);
        }
	/*else if(strcmp(dp->d_name, ".") == 0)
	{
		printf("dot");
	}
	else
	{
		printf("doubledot");
	}*/
    }
    closedir(dir);
}
