#ifndef DEFERREDTASKSEXECUTOR_H
#define DEFERREDTASKSEXECUTOR_H
#include <mutex>
#include <vector>
#include "deferredtask.h"

class DeferredTasksExecutor
{
public:
    DeferredTasksExecutor();
    DeferredTasksExecutor(int tasks);
    ~DeferredTasksExecutor();
    int addTask(DeferredTask *task);
    int getTaskStatus(int task_id);
private:
    static void worker(void *that);
    void realWorker();

private:
    std::mutex m_worker_mutex;
    int m_workers;
    std::vector<DeferredTask> m_tasks;
    std::vector<DeferredTask>::iterator m_undoneIterator = m_tasks.begin(); // Really pointed to m_tasks.end();
};

#endif // DEFERREDTASKSEXECUTOR_H
