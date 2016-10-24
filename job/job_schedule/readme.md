# 操作系统作业调度实验报告

 姓名 | 学号 
------|------
朱承浩| 2014210761

## 一: 实验目的
1. 熟练使用各种作业调度算法描述的过程
2. 掌握各种算法的优缺点
3. 掌握作业调度的基本思想
4. 提高理论和实践结合的能力

## 二: 实验内容
1. 先来先服务算法
2. 最短作业优先算法
3. 最高响应比优先算法

## 三: 实验环境
+ 实践平台: Linux
+ 编写环境: Vim
+ 编译器: g++
+ 调试器: gdb

## 四: 实验设计原理
### 1.先来先服务算法
根据作业输入输入井的先后顺序调度作业即先到达输入井的作业先被调度，后到达的作业后调度.

### 2.最短作业算法
根据作业需要运行的时间的长短来确定调用哪一个作业，即当前输入井中需要执行时间越短越早执行.

### 3.最高响应比优先算法
输入井中的每一个作业根据当前运行的作业计算出作业的响应比，响应比最高的将被调度.

## 五: 实验详细实现过程与算法流程
### 1.先来先服务
先来先服务非常适合使用队列结构: 先进先出. 

### 2.最短作业优先
最短作业优先需要对作业列表以执行时间排序. 同时需要设置系统时间计数,
判断某个作业是否进入输入井, 从而决定能否执行.

### 3.最高响应比优先
最高响应比优先需要计算每个作业的最高响应比并对作业列表排序. 同时需要设置系统时间计数,
判断某个作业是否进入输入井, 从而决定能否执行.

## 六: 实验调试与结果
### gdb调试
出现```float point exception```, 使用gdb调试, 原来是rand模拟rtime的时候出现了0.
<br/>
![gdb](https://cloud.githubusercontent.com/assets/10671733/19182622/c53560d4-8ca6-11e6-9b06-c3aba81ea176.png)

### g++编译结果
运行结果: <br/>
![运行结果](https://cloud.githubusercontent.com/assets/10671733/19183772/7e3c45de-8cac-11e6-9013-38ced34b9317.png)

## 七: 源程序(含注释)
```
/***
 * 操作系统调度算法
 * ----------------
 *     用户输入总进程数, 程序随机模拟每个进程的JCB
 *     输出每个算法的调度信息(单位: 分钟)
 */
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
```

## 八: 实验结果分析
从实验结果来看: <br/>

+ 先来先服务算法的效率最差, 的确如果刚进入的服务是一个耗时程序(比如I/O), 会使其他短执行时间的任务无法执行, 系统利用率低.
+ 短作业优先的系统利用率高, 可以执行很多任务, 但是长执行时间的任务可能一直无法被执行
+ 从结果来看, 最高响应比很好的做了折衷, 既优先执行短执行时间的任务, 同时也兼顾等待时间长的任务, 是比较理想的调度算法.
