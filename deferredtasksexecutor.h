#ifndef DEFERREDTASKSEXECUTOR_H
#define DEFERREDTASKSEXECUTOR_H

#include <atomic>
#include <mutex>
#include <vector>
#include <queue>
#include "deferredtask.h"

class DeferredTasksExecutor
{
public:
    DeferredTasksExecutor();
    DeferredTasksExecutor(int taskCount);
    ~DeferredTasksExecutor();
    int addTask(Task & _task, int priority);
    bool cancelTask(int taskId);
    int getTaskStatus(int taskId);
private:
    static void worker(void *that);
    void realWorker();
    void terminateWorkers();

private:
    typedef std::priority_queue<DeferredTask,std::vector<DeferredTask>, Comparer>  TaskPool;
    std::mutex m_worker_mutex;
    int m_workers;
    std::vector<std::thread> m_threads;
    TaskPool m_tasks;
    int m_lastTaskId = 0;
    TaskPool::iterator m_undoneIterator = m_tasks.begin(); // Really pointed to m_tasks.end();
    bool m_terminate = false;
};

#endif // DEFERREDTASKSEXECUTOR_H
