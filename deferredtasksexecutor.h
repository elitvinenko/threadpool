#ifndef DEFERREDTASKSEXECUTOR_H
#define DEFERREDTASKSEXECUTOR_H

#include <atomic>
#include <mutex>
#include <vector>
#include <queue>
#include <boost/heap/priority_queue.hpp>
#include "deferredtaskpool.h"
#include "deferredtask.h"

class Comparer
{
public:
        bool operator() (const DeferredTask lhs, const DeferredTask rhs);
};

class DeferredTasksExecutor
{
public:
    DeferredTasksExecutor();
    DeferredTasksExecutor(unsigned int taskCount);
    ~DeferredTasksExecutor();
    int addTask(Task & task, int priority);
    bool cancelTask(int taskId);
    int getTaskStatus(int taskId);
    const std::vector<int> & getTasksPriority();
    void execute();
    void wait();
private:
    static void worker(void *that);
    void realWorker();
    void terminateWorkers();

private:
    DeferredTaskPool taskpool;
    int m_workers;
    std::vector<std::thread> m_threads;
    int m_lastTaskId = 0;
    bool m_terminate = false;
    unsigned int  m_maxThreads;
};

#endif // DEFERREDTASKSEXECUTOR_H
