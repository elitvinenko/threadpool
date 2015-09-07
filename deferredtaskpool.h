#ifndef DEFERREDTASKPOOL_H
#define DEFERREDTASKPOOL_H

#include "deferredtask.h"
#include "task.h"
#include <list>
#include <mutex>

class DeferredTaskPool
{
public:
    DeferredTaskPool();
    int addTask(Task & _task, int priority);
    bool cancelTask(int taskId);
    Task *getTask();
    bool setDoneStatus(int taskId);
    int getTaskStatus(int taskId);
    void printfPoll();
protected:
    typedef std::list<DeferredTask> Pool;
    Pool  m_tasks;
    int m_lastTaskId = 0;
    Pool::iterator m_undoneIterator = m_tasks.begin(); // Really pointed to m_tasks.end();
    std::mutex m_worker_mutex;
};

#endif // DEFERREDTASKPOOL_H
