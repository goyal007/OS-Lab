#include<bits/stdc++.h>
using namespace std;
#define MAX_TICKET 100
int main(int argc,char **argv)
{
    int proc;
    cout<<"Enter total number of processes:\n";
    cin>>proc;
    
    int prob[proc];
    for(int i=0;i<proc;i++)
    {  
        prob[i]=0;
    }
    
    
    vector<pair<int,int> > group;
    cout<<"Enter each process proportion:\n";
    int a=1,b;
    for(int i=0;i<proc;i++)
    {
        cin>>b;
        //cout<<b;
        b=a+b-1;
        group.push_back(make_pair(a,b));
        a=b+1;
    }
    int size=group.size();
    /*for(int i=0;i<group.size();i++)
    {
        cout<<"("<<group[i].first<<","<<group[i].second<<")";
    }*/
    for(int i=0;i<100;i++)
    {
        int ticket=rand()%100;
        
        for(int j=0;j<group.size();j++)
        {
            int fir=group[j].first;
            int sec=group[j].second;
            //cout<<"first"<<fir<<"   "<<"   second"<<sec;
            if(ticket>=fir && ticket<=sec)
            {
                prob[j]++;
                cout<<"Ticket number::"<<i+1<<"\t"<<ticket<<"\t"<<"Process"<<j+1<<endl;
                break;
            }
        }
    }
    float sum;
    for(int i=0;i<proc;i++)
    {  
        float a1=prob[i]*1.0/100;
        sum+=a1;
        cout<<"Probability of processor:"<<i+1<<" "<<a1<<endl;
    }
    cout<<"Sum of all Probability:"<<sum<<endl;
    return 0;
}
