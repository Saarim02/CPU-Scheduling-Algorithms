#include<bits/stdc++.h>
using namespace std;

class Process{
public:
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rt;

    Process(int i, int j, int k){
        pid = i;
        at = j;
        bt = k;
        ct = -1;
        tat = -1;
        wt = -1;
        rt = bt;
    }
};

int cur_time = 0;

// Custom comparator for sorting by arrival time and PID
bool cmp_at(Process* p1, Process* p2){
    if(p1->at > p2->at) return false;
    if(p1->at == p2->at) return p1->pid < p2->pid;
    return true;
}

// Custom comparator for priority queue based on response ratio
class Compare{
public:
    bool operator()(Process* p1, Process* p2){
        if(ResponseRatio(p1) < ResponseRatio(p2)) return true;
        if(ResponseRatio(p1) == ResponseRatio(p2)) return p1->pid > p2->pid;
        return false;
    }

    static float ResponseRatio(Process* p1){
        int waiting_time = cur_time - p1->at;
        return (float)(waiting_time + p1->bt) / p1->bt;
    }
};

void generateParams(vector<Process*>& process){
    for(int i = 0; i < process.size(); i++){
        process[i]->tat = process[i]->ct - process[i]->at;
        process[i]->wt = process[i]->tat - process[i]->bt;
    }
}

void HRRN(vector<Process*>& process){
    sort(process.begin(), process.end(), cmp_at);
    priority_queue<Process*, vector<Process*>, Compare> ready;

    int timer = process[0]->at;
    cur_time = timer;

    // Adding initial processes that have arrived by the time the first process arrives
    for(int i = 0; i < process.size(); i++){
        if(process[i]->at > timer) break;
        ready.push(process[i]);
    }

    while(!ready.empty()){
        auto execution = ready.top();
        ready.pop();
        
        timer += execution->bt;
        execution->rt = 0;
        execution->ct = timer;

        cur_time = timer;

        cout << "Executing process: " << execution->pid << " | ";

        // Add newly arrived processes that are ready
        for(int l = 0; l < process.size(); l++){
            if(process[l]->at <= timer && process[l]->rt > 0){
                ready.push(process[l]);
            }
        }
    }

    cout << endl << endl;
    generateParams(process);

    int total_tat = 0, total_wt = 0, schedule_length = 0;
    cout << "Pid " << "A.T " << "B.T " << "C.T " << "T.A.T " << "W.T" << endl;
    for(auto i : process){
        schedule_length = max(schedule_length, i->ct);
        total_tat += i->tat;
        total_wt += i->wt;
        cout << i->pid << "   | " << i->at << "   | " << i->bt << "   | " << i->ct << "   | " << i->tat << "   | " << i->wt << endl;
    }

    cout << endl;
    cout << "AVERAGE T.A.T -> " << (float)total_tat / process.size() << endl;
    cout << "AVERAGE W.T -> " << (float)total_wt / process.size() << endl;
    cout << "Scheduling Length -> " << schedule_length << endl;
}

int main(){
    int n;
    cout<<"Enter the no. of process";
    cin >> n;
    vector<Process*> process;
    cout<<"Enter pid, arrival time, burst time of each process"<<endl;
    for(int i = 0; i < n; i++){
        int pid, at, bt;
        cin >> pid >> at >> bt;
        Process* p1 = new Process(pid, at, bt);
        process.push_back(p1);
    }

    HRRN(process);
    return 0;
}
