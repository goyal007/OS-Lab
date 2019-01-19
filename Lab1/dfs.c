#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
void dfs(char *basePath, const int root);
int main()
{
    char path[100];
    printf("Enter path: ");
    scanf("%s", path);
    dfs(path, 0);
    return 0;
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
