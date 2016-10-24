#include <iostream>
#include <list>
#include <string>
#include <cstdlib>

using namespace std;

struct Job {
    // 模拟JCB
    unsigned int jid;    // 作业id
    unsigned int ctime;  // 作业进入输入井时间
    unsigned int rtime;  // 作业预计执行时间
    unsigned int start;  // 作业开始执行时间
    unsigned int stop;   // 作业结束执行时间
    unsigned int wtime;  // 作业等待时间
    unsigned int T;      // 作业的周转时间(stop-ctime)
    float W;             // 作业的带权周转时间[(stop-ctime)/rtime]
};

bool compare_rtime(Job& joba, Job& jobb) {
    // list rtime排序比较函数
    if (joba.rtime < jobb.rtime) { return true; }
    else                         { return false; }
}

bool compare_R(Job& joba, Job& jobb) {
    // list R排序比较函数
    if (joba.wtime/joba.rtime > jobb.wtime/jobb.rtime) { return true; }
    else                                               { return false; }
}

list<Job> fcfs(list<Job> jobs) {
    // 先来先服务算法, 默认按作业号进入输入井
    unsigned int time = jobs.front().ctime; // 系统时间
    list<Job> fcfs_jobs;  // 调度队列
    for(auto&& job : jobs) {
        job.start = time;
        job.stop = job.start + job.rtime;
        job.T = job.stop - job.ctime;
        job.W = (float)(job.T / job.rtime);
        time += job.rtime;
        fcfs_jobs.push_back(job);
    }
    return fcfs_jobs;
}

list<Job> sjf(list<Job> jobs) {
    // 短作业优先, 按rtime排序, 需要考虑其他进程还有没有开始进入
    unsigned int time = 0;   // 系统时间
    list<Job> sjf_jobs;     // 调度队列

    // j1作业最先进入, 放入调度队列, 并从jobs队列中移除
    jobs.front().start = jobs.front().ctime;
    jobs.front().stop = jobs.front().start + jobs.front().rtime;
    jobs.front().T = jobs.front().stop - jobs.front().ctime;
    jobs.front().W = (float)((jobs.front().T) / jobs.front().rtime);
    time += jobs.front().rtime;
    sjf_jobs.push_back(jobs.front());
    jobs.pop_front();
    
    list<Job>::iterator it = jobs.begin();
    while(jobs.size()) {
       // 对剩余任务进行rtime排序
       // 符合已进入输入井且rtime小的作业放入可执行队列
       jobs.sort(compare_rtime);
       for (list<Job>::iterator it=jobs.begin(); it!=jobs.end(); ++it) {
           Job job = *it;
           if (job.ctime < time) {
               // 已经进入输入井
               job.start = time;
               job.stop = job.start + job.rtime;
               job.T = job.stop - job.ctime;
               job.W = (float)(job.T / job.rtime);
               time += job.rtime;
               sjf_jobs.push_back(job);
               // jobs.remove(job);
               jobs.erase(it);
               continue;
           }
       }
    }
    return sjf_jobs;
}

list<Job> hrn(list<Job> jobs) {
    unsigned int time = 0;   // 系统时间
    list<Job> hrn_jobs;     // 调度队列

    // j1作业最先进入, 放入调度队列, 并从jobs队列中移除
    jobs.front().start = jobs.front().ctime;
    jobs.front().stop = jobs.front().start + jobs.front().rtime;
    jobs.front().T = jobs.front().stop - jobs.front().ctime;
    jobs.front().W = (float)((jobs.front().T) / jobs.front().rtime);
    time += jobs.front().rtime;
    hrn_jobs.push_back(jobs.front());
    jobs.pop_front();
    
    list<Job>::iterator it = jobs.begin();
    while(jobs.size()) {
       // 对剩余任务进行rtime排序
       // 符合已进入输入井且rtime小的作业放入可执行队列
       for (auto&& job : jobs) { job.wtime = time - job.ctime; }
       jobs.sort(compare_R);
       for (list<Job>::iterator it=jobs.begin(); it!=jobs.end(); ++it) {
           Job job = *it;
           if (job.ctime < time) {
               // 已经进入输入井
               job.start = time;
               job.stop = job.start + job.rtime;
               job.T = job.stop - job.ctime;
               job.W = (float)(job.T / job.rtime);
               time += job.rtime;
               hrn_jobs.push_back(job);
               // jobs.remove(job);
               jobs.erase(it);
               continue;
           }
       }
    }
    return hrn_jobs;
}

void schedule_info(list<Job> jobs, string schedule) {
    // 调度信息输出框架
    int total = jobs.size();
    float Tsum = 0.0;
    float Wsum = 0.0;

    cout << "\n---> " << schedule << endl;
    cout << "total jobs: " << total << endl;
    cout << "jid\t" << "ctime\t" << "rtime\t" << "start\t" << "stop\t" << "W\t" << "T" << endl;
    for (auto&& job : jobs) {
        Tsum += job.T; Wsum += job.W;
        cout << job.jid << "\t" << job.ctime << "\t";
        cout << job.rtime << "\t" << job.start << "\t" << job.stop << "\t";
        cout << job.W << "\t" << job.T << endl;
    }
    cout << "pTsum: " << Tsum/(float)total << endl;
    cout << "pWsum: " << Wsum/(float)total << endl;
    cout << endl;
}

int main(void) {
    // 进程模拟, 信息输出框架
    unsigned int total = 0;
    list<Job> jobs; // 作业列表
    cout << "Total number of jobs: ";
    cin >> total;

    for(int i=0; i<total; i++) {
        // 初始化作业信息
        Job job;
        job.jid = i+1;
        job.ctime = i*2;  // 默认p1就是先进入输入井的作业
        job.rtime = rand()%10+1;
        jobs.push_back(job);
    }

    list<Job> fcfs_jobs = fcfs(jobs);     // 先来先服务
    list<Job> sjf_jobs = sjf(jobs);       // 短作业优先
    list<Job> hrn_jobs = hrn(jobs);       // 最高响应比

    // 输出3种算法的调度信息
    schedule_info(fcfs_jobs, "fcfs");
    schedule_info(sjf_jobs, "sjf");
    schedule_info(hrn_jobs, "hrn");

    return 0;
}
