#ifndef DEFERREDTASKSEXECUTOR_H
#define DEFERREDTASKSEXECUTOR_H
#include <mutex>
#include <vector>
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

private:
    std::mutex m_worker_mutex;
    int m_workers;
    std::vector<std::thread> m_threads;
    std::vector<DeferredTask> m_tasks;
    int m_lastTaskId = 0;
    std::vector<DeferredTask>::iterator m_undoneIterator = m_tasks.begin(); // Really pointed to m_tasks.end();
};

#endif // DEFERREDTASKSEXECUTOR_H
