#include <stdio.h>
#include<stdlib.h>
#include <dirent.h>
#include <string.h>
#include<stdlib.h>
#include <sys/types.h>
struct node
{
	char path[1000];
	int level;
	struct node *link;
};
struct node* START=NULL;
struct node* create_node()
{
	struct node *n;
	n=(struct node *)malloc(sizeof(struct node));
	return n;
}
void insert_node(char* pth,int lvl)
{
	struct node *temp,*t;
	temp=create_node();
	strcpy(temp->path,pth);
	temp->link=NULL;
	temp->level=lvl;
	if (NULL==START)
	{
		START=temp;
	}
	else
	{
		t=START;
		while(t->link!=NULL)
		{
			t=t->link;
		}
		t->link=temp;
	}

}
struct node* first_delet()
{
	struct node *r;
	if(NULL==START)
	{
		return(START);
	}
	else
	{
		r=START;
		START=START->link;
		//free(r);
		return(r);
	}
}
void show_dir_content()
{
	struct node* t = first_delet();
	int c=1;
  while(t != NULL)
  {
  		if(c==1)
  		{
  			c++;
  		}
  		else
  		{
  			t = first_delet();
  		}

  	if(t ==NULL)
  	{
  		break;
  	}
  	int x=t->level;
	  DIR * d = opendir(t->path);
	  if(d==NULL)
	  	return;
	  struct dirent * dir;
	  while ((dir = readdir(d)) != NULL)
		{
		  if(dir->d_type != DT_DIR)
		    printf("%d->%s\n",x+1, dir->d_name);
		  else if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 )
		  {
		    printf("%d->%s\n",x+1, dir->d_name);
		    char* d_path = (char*)malloc(255*sizeof(char));
		    sprintf(d_path, "%s/%s", t->path, dir->d_name);
		    insert_node(d_path,x+1);
		  }
		}
		closedir(d);
	}

}

int main()
{
	char p[5000];
	scanf("%s",p);
	printf("%s\n",p);
	insert_node(p,0);
    show_dir_content();
  return(0);
}
