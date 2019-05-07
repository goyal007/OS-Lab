#include<stdio.h>
#include<limits.h>
int absolute_val(int a)
{
	if(a<0)
	{
		return -1*a;
	}
	else
	{
		return a;
	}
}
int main()
{
	int requests,head_start,i,total_seek,t1,t2,j;
	
	printf("Enter total Requests");
	scanf("%d",&requests);
	int arr[requests];
	printf("Enter Track number\n");
	for(i=0;i<requests;i++)
	{
		scanf("%d",&arr[i]);
	}
	printf("Enter Start head position\n");
	scanf("%d",&head_start);

	printf("Enter Disk size\n");
	scanf("%d",&t2);

	t1=0;
	//t2=t2-1;
	total_seek=0;
	int c=0,temp;
	while(1)
	{
		c=0;
		int index,min_val=INT_MAX;
		for(i=0;i<requests;i++)
		{
			if(arr[i]!=-100)
			{
				temp=absolute_val(arr[i]-head_start);
				if(temp<min_val)
				{
					min_val=temp;
					index=i;
				}
			}
			else
			{
				c++;
			}
		}
		if(c==requests)
		{
			break;
		}
		else
		{
			total_seek+=min_val;
			printf("requests entertained from %d to %d track\n",head_start,arr[index]);
			head_start=arr[index];
			arr[index]=-100;
			
			
		}
		
	}
		


//95 180 34 119 11 123 62 64

	printf("Total seek movements are : %d\n",total_seek);
	
}
