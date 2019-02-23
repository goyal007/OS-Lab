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
				check.insert(page);
				fifo.push(page);
				cout<<"\tmiss\n";
				faults++;
			}
			else
			{
				cout<<"\thit\n";
				hit++;
			}
		}
		else 
		{
			if(check.find(page)==check.end())
			{
				temp=fifo.front();
				cout<<"\tevicted page::"<<temp<<endl;
				fifo.pop();
				fifo.push(page);
				check.erase(temp);
				check.insert(page);
				cout<<"\tmiss\n";
				faults++;
			}
			else
			{
				cout<<"\thit\n";
				hit++;
			}
		}
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
