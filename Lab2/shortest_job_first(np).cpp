#include<bits/stdc++.h>
using namespace std;
int total_process,system_time;
class Process_class
{
	public:
		int pid,arrival_time,priority,share,response_t,turn_around_t,wait_t,overall_burst;
		
		vector < pair< int,int > > burst_time;
		int counter_burst,remaing_time;
		int first_response_start;
};
Process_class process[20];
vector <Process_class* > process_queue;
queue <Process_class* > input_queue;
queue <Process_class* > output_queue;
bool comparator_arrivalTime(Process_class p1,Process_class p2)
{
	return (p1.arrival_time < p2.arrival_time);
		
}
bool comparator_burstTime(Process_class* p1,Process_class* p2)
{
	if(p1->burst_time[p1->counter_burst].second  <  p2->burst_time[p2->counter_burst].second)
		return true;
	return false;
}
void helperFunction()
{
	
	//cout<<"ashish in helper\t";
	int first_time_process=0;
	while(true)
	{
		cout<<"system time:"<<system_time<<"\n"<<"total process:"<<total_process<<"\n";
		while(first_time_process<total_process && process[first_time_process].arrival_time==system_time)
		{
		
			process_queue.push_back(&process[first_time_process]);
			process[first_time_process].counter_burst=0;
			process[first_time_process].remaing_time=process[first_time_process].burst_time[process[first_time_process].counter_burst].second;
			first_time_process=first_time_process+1;
		}

		if(  !process_queue.empty() && process_queue.front()->remaing_time==0)
		{
		
			if(process_queue.front()->counter_burst==process_queue.front()->burst_time.size()-1)
			{	
			
				process_queue.front()->turn_around_t=system_time - process_queue.front()->arrival_time;
			}
			else if(process_queue.front()->burst_time[process_queue.front()->counter_burst+1].first=='O' )
			{
				output_queue.push(process_queue.front());
				process_queue.front()->remaing_time=process_queue.front()->burst_time[process_queue.front()->counter_burst+1].second;
				process_queue.front()->counter_burst++;
			}
			else if (process_queue.front()->burst_time[process_queue.front()->counter_burst+1].first=='I' )
			{
				input_queue.push(process_queue.front());
				process_queue.front()->remaing_time=process_queue.front()->burst_time[process_queue.front()->counter_burst+1].second;
				process_queue.front()->counter_burst++;
			}
			
			process_queue.erase(process_queue.begin());
			sort(process_queue.begin(),process_queue.end(),comparator_burstTime);
		}
		if(  !input_queue.empty() && input_queue.front()->remaing_time==0)
		{
			if(input_queue.front()->counter_burst==input_queue.front()->burst_time.size()-1)
			{
				input_queue.front()->turn_around_t=system_time - input_queue.front()->arrival_time;
			}
			else if(input_queue.front()->burst_time[input_queue.front()->counter_burst+1].first=='O' )
			{	
				output_queue.push(input_queue.front());
				input_queue.front()->remaing_time=input_queue.front()->burst_time[input_queue.front()->counter_burst+1].second;
				input_queue.front()->counter_burst++;
			}
			else if (input_queue.front()->burst_time[input_queue.front()->counter_burst+1].first=='C' )
			{
				process_queue.push_back(input_queue.front());
				input_queue.front()->remaing_time=input_queue.front()->burst_time[input_queue.front()->counter_burst+1].second;
				input_queue.front()->counter_burst++;
			}
			 
			input_queue.pop();
		}
		if(  !output_queue.empty() && output_queue.front()->remaing_time==0)
		{
			
			if(output_queue.front()->counter_burst==output_queue.front()->burst_time.size()-1)
			{	
				//cout<<"termi:"<<output_queue.front()->counter_burst<<endl;
				output_queue.front()->turn_around_t=system_time - output_queue.front()->arrival_time;
			}
			else if(output_queue.front()->burst_time[output_queue.front()->counter_burst+1].first=='I' )
			{	
				input_queue.push(output_queue.front());
				output_queue.front()->remaing_time=output_queue.front()->burst_time[output_queue.front()->counter_burst+1].second;
				output_queue.front()->counter_burst++;
			}
			else if (output_queue.front()->burst_time[output_queue.front()->counter_burst+1].first=='C' )
			{
				process_queue.push_back(output_queue.front());
				output_queue.front()->remaing_time=output_queue.front()->burst_time[output_queue.front()->counter_burst+1].second;
				output_queue.front()->counter_burst++;
			}
			

			output_queue.pop();
		}


		if(input_queue.empty()  && process_queue.empty() &&output_queue.empty() && total_process==first_time_process)
		{
			
			break;
		}
			
		if(!input_queue.empty())
		{
			
			input_queue.front()->remaing_time=input_queue.front()->remaing_time-1;
			printf("i/p queue   %d %d\n",input_queue.front()->pid,input_queue.front()->remaing_time);
		}
		if(!process_queue.empty())
		{	
			
			if(process_queue.front()->first_response_start==0)
			{
				process_queue.front()->response_t=system_time - process_queue.front()->arrival_time;
				process_queue.front()->first_response_start=1;
			}
			process_queue.front()->remaing_time=process_queue.front()->remaing_time-1;
			printf("p queue   %d %d\n",process_queue.front()->pid,process_queue.front()->remaing_time);
		}
		if(!output_queue.empty())
		{
			
			output_queue.front()->remaing_time=output_queue.front()->remaing_time-1;
			printf("o/p queue   %d %d\n",output_queue.front()->pid,output_queue.front()->remaing_time);
		}

		system_time=system_time+1;
	}
}


int main(int argc,char **argv)
{	
	FILE* fp = fopen("input.dat","r");
	char temp;
	char operation;
	int burst;
	
	fscanf(fp,"%d",&total_process);
	for(int i=0;i<total_process;i++)
	{
		fscanf(fp,"%d%d%d%d",&process[i].pid,&process[i].arrival_time,&process[i].priority,&process[i].share);
		fscanf(fp,"%c",&temp);
		while(true)
		{
			fscanf(fp,"%c",&operation);
			//cout<<operation<<"\t";
			if(operation == '-')
			{
				break;
			}
			fscanf(fp,"%d",&burst);
			//cout<<"("<<operation<<","<<burst<<")"<<"\t";
			process[i].burst_time.push_back(make_pair(operation,burst));
			fscanf(fp,"%c",&temp);
		}
	}
	
	//cout<<"ashish\t";
	for(int i=0;i<total_process;i++)
	{
		process[i].first_response_start = 0;
		process[i].remaing_time=0;
		process[i].counter_burst=0;
	}




	sort(process,process+total_process,comparator_arrivalTime);
	//cout<<"ashish\t";




	//process[0].first_response_start=1;
	//process[0].response_t=0;




	helperFunction();
	//cout<<"ashish\t";
	int count = 0;
	for(int i=0;i<total_process;i++)
	{
		count=0;
		for(int j=0;j<process[i].burst_time.size();j++)
		{
			count=count+ process[i].burst_time[j].second;
		}
		process[i].overall_burst = count;
		cout<<"total burst:"<<process[i].overall_burst<<endl;
	}
	
	
	for (int i = 0; i < total_process; ++i)
	{
		process[i].wait_t=process[i].turn_around_t - process[i].overall_burst;
	}
	
	float average_rt,average_wt,average_tat;
	for (int i = 0; i < total_process; ++i)
	{
		
		average_tat+=process[i].turn_around_t;
		average_wt+=process[i].wait_t;
		average_rt=process[i].response_t;
	}
	average_rt/=total_process;
	average_wt/=total_process;
	average_tat/=total_process;

	cout<<"pid\tarr_t\tresp_t\twait_t\tturn_at\n";
	for(int i=0;i<total_process;i++)
		cout<<process[i].pid<<"\t"<<process[i].arrival_time<<"\t"<<process[i].response_t<<"\t"<<process[i].wait_t<<"\t"<<process[i].turn_around_t<<"\n";
		

	cout<<endl;


//	cout<<"average response time:"<<average_rt<<endl;
//	cout<<"average wait time:"<<average_wt<<endl;
//	cout<<"average turn around time:"<<average_tat<<endl;

	return 0;
}
