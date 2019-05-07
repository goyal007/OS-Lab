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
		
	for(i=head_start;i<t2;i++)
	{
		for(j=0;j<requests;j++)
		{
			if(i==arr[j])
			{
				int temp_diff=absolute_val(head_start-i);
				arr[j]=-100;
				printf("requests entertained from %d to %d track\n",head_start,i);
				total_seek=total_seek+temp_diff;
				head_start=i;
			}
		}
	}

	int temp_diff=absolute_val(head_start-(t2-1));
	printf("no requests entertained from %d to %d track\n",head_start,t2-1);
	total_seek=total_seek+temp_diff;
	head_start=t2-1;

	for(i=t2-1;i>=0;i--)
	{
		for(j=0;j<requests;j++)
		{
			if(i==arr[j])
			{
				int temp_diff=absolute_val(head_start-i);
				arr[j]=-100;
				printf("requests entertained from %d to %d track\n",head_start,i);
				total_seek=total_seek+temp_diff;
				head_start=i;
			}
		}
	}

	// temp_diff=absolute_val(head_start-0);
	// printf("requests entertained from %d to %d track\n",head_start,0);
	// total_seek=total_seek+temp_diff;
	// head_start=0;

	printf("Total seek movements are : %d\n",total_seek);
	
}
