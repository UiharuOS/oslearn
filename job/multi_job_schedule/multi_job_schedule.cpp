/***
 *      multi_job_schedule
 *      ``````````````````
 *          preemptive sjf schedule
 */

#include <iostream>
#include <list>

using namespace std;

struct Job {
    // 模拟JCB
    unsigned int jid;     // 作业id
    unsigned int ctime;   // 作业进入输入井的时间
    unsigned int rtime;   // 作业执行时间
    unsigned int start;   // 作业开始执行时间
    unsigned int stop;    // 作业停止执行时间
    unsigned int wtime;   // 作业剩余执行时间
};

bool compare_rtime(Job& job1, Job& job2) {
    // 比较rtime
    if (job1.rtime < job2.rtime) { return true; }
    else                         { return false; }
}

void start_execute(Job& job, unsigned int& time) {
    // 开始执行作业 
    job.start = time;
}

void con_execute(Job& job, unsigned int& time) {
    // 继续执行挂起的作业
    job.stop = time + job.wtime;
    cout << job.start << "~" << (job.stop-job.wtime) << ":\t" << job.jid << endl;
    cout << (job.stop-job.wtime) << "~" << job.stop << ":\t" << job.jid << endl;
    job.wtime = 0;
    time = job.stop;
}

void wait(Job& job, unsigned int& time) {
    // 挂起作业, 计算wtime
    if (job.start == 0) {
        // 还没启动
        job.wtime = job.rtime;
    }
    else {
        job.wtime = job.rtime - (time - job.start);
    }
}

void execute(Job& job, unsigned int& time) {
    job.start = time;
    job.stop = job.start + job.rtime;
    job.wtime = 0;
    cout << job.start << "~" << job.stop << ":\t" << job.jid << endl;
    time = job.stop;
}

void multi_sjf(list<Job> jobs) {
    // 2道短作业优先调度, 输出每个任务的调度信息
    list<Job> execute_jobs;      // 可执行作业队列
    unsigned int time = 0;       // 系统时间(从0开始)

    Job job1 = jobs.front();
    execute_jobs.push_back(job1);
    jobs.pop_front();
    time = job1.ctime;
    start_execute(job1, time);

    Job job2 = jobs.front();
    execute_jobs.push_back(job2);
    jobs.pop_front();
    time = job2.ctime;  // job2进入的时间当前系统时间
    wait(job1, time);   // 挂起job1

    if (job1.wtime < job2.rtime) {
        con_execute(job1, time);
        execute_jobs.pop_front();
    }
    else {
        execute(job2, time);
        execute_jobs.pop_back();
    }
    
    jobs.sort(compare_rtime); // 根据rtime排序
    for(list<Job>::iterator it=jobs.begin(); it!=jobs.end(); ++it) {
        Job job = *it;
        if(job.ctime <= time && execute_jobs.size()==0) {
            execute_jobs.push_back(job);
            execute(job, time); // sjf
            execute_jobs.pop_back();
            jobs.erase(it);
        }
        else if(job.ctime <= time && execute_jobs.size()==1) {
            execute_jobs.push_back(job);
            Job current_job = execute_jobs.front();
            // current_job.wtime = current_job.rtime - (time - current_job.start);
            wait(current_job, time);          // 挂起当前作业
            if(current_job.wtime < job.rtime) {
                con_execute(current_job, time);
                execute_jobs.pop_front();
                jobs.erase(it);
            }
            else {
                execute(job, time);           // 执行新的作业
                execute_jobs.pop_back();
                jobs.erase(it);
            }
        }
        else {}  //等待
    }

    if (jobs.size() != 0) {
        // 后进入输入井作业
        // jobs.sort(compare_rtime);
        for(auto&& job : jobs) {
            execute(job, time);
        }
    }

    if (execute_jobs.size() != 0) {
        Job job = execute_jobs.front();
        execute(job, time);
        execute_jobs.pop_front();
    }
}

int main(void) {
    list<Job> jobs;      // 输入井
    unsigned int total;  // 总进程数
    cout << "Total number of jobs: ";
    cin >> total;
    for(int i=0; i<total; ++i) {
        /*初始化作业~随机数*/
        Job job;
        job.jid = i+1;
        job.ctime = (i+1)*2;        // 默认按jid顺序进入输入井
        job.rtime = rand()%10 + 1;  // 作业执行时间(1~10)
        job.wtime = job.rtime;
        jobs.push_back(job);        // 作业进入输入井
    }

    // 2道短作业优先调度
    cout << "--> preemptive sjf schdule" << endl;
    cout << "total job number: " << total << endl;
    cout << endl;
    for(auto&& job: jobs) {
        cout << "jid\t" << "ctime\t" << "rtime\t" << "wtime\t" << endl;
        cout << job.jid << "\t" << job.ctime << "\t" << job.rtime << "\t" << job.wtime << endl;
    }
    cout << "-------------------------------" << endl;
    cout << "time\t" << "jid" << endl;
    cout << "(start~end)" << endl;
    cout << "-------------------------------" << endl;
    multi_sjf(jobs);
    cout << endl;

    return 0;
}
