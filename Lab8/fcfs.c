#include<stdio.h>
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
	int requests,head_start,i,total_seek;
	
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
	total_seek=0;
	for(i=0;i<requests;i++)
	{
		int temp_diff=absolute_val(head_start-arr[i]);
		printf("requests entertained from %d to %d track\n",head_start,arr[i]);
		total_seek=total_seek+temp_diff;
		head_start=arr[i];
	}

	printf("Total seek movements are : %d",total_seek);
	
}
