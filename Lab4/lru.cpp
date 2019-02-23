#include<bits/stdc++.h>
using namespace std;
int main()
{
	int buffer_size;
	cout<<"enter cache buffer size::";
	cin>>buffer_size;
	int page;
	page=INT_MAX;
	//take input -1 to stop page numbers
	unordered_set<int> check; 
	queue<int> fifo; 
	int faults=0;
	int temp;
	int count=0,hit=0;
	int arr[buffer_size];
	for(int i=0;i<buffer_size;i++)
	{
		arr[i]=-2;
	}
	
	int index=0;
	while(page!=-1)
	{
		cout<<"Enter page value::";
		cin>>page;
		if(page==-1)
			break;
		if(check.size()<buffer_size)
		{
			if(check.find(page)==check.end())
			{
				arr[index]=page;
				index++;
				if(index==buffer_size)
				{
					index=buffer_size;
				}
				check.insert(page);
				faults++;
				
				cout<<"\tmiss\n";
			}
			else
			{
				int temp_index;
				for(int i=0;i<index;i++)
				{
					if(page==arr[i])
					{
						temp_index=i;
						break;
					}
				}
				int j;
				for( j=temp_index;j<index-1;j++)
				{
					arr[j]=arr[j+1];
				}
				arr[j]=page;
				cout<<"\thit\n";
				hit++;
			}
		}
		else 
		{
			if(check.find(page)==check.end())
			{
				temp=arr[0];
				cout<<"\tevicted page::"<<temp<<endl;
				arr[0]=-2;
				int i;
				for( i=0;i<buffer_size-1;i++)
				{
					arr[i]=arr[i+1];
				}
				arr[i]=page;
				check.erase(temp);
				check.insert(page);
				cout<<"\tmiss\n";
				faults++;
			}
			else
			{
				int temp_index;
				for(int i=0;i<buffer_size;i++)
				{
					if(page==arr[i])
					{
						temp_index=i;
						break;
					}
				}
				int j;
				for( j=temp_index;j<buffer_size-1;j++)
				{
					arr[j]=arr[j+1];
				}
				arr[j]=page;
				
				
				cout<<"\thit\n";
				hit++;
			}
		}
		
		for(int i=0;i<index;i++)
		{
			cout<<arr[i]<<" ";
		}
		cout<<endl;
		count++;
	}
	
	float hr,ms;
	cout<<"Page Faults ::"<<faults<<endl;
	cout<<"Hits ::"<<hit<<endl;
	ms=faults*1.0/count;
//	hr=1-ms;
	hr=hit*1.0/count;
	cout<<"Hit Ratio ::"<<hr<<endl;
	cout<<"Miss Ratio ::"<<ms<<endl;	
	return 0;
}
