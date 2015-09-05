#ifndef DEFERREDTASKSEXECUTOR_H
#define DEFERREDTASKSEXECUTOR_H
#include <deque>
#include "deferredtask.h"

class DeferredTasksExecutor
{
public:
    DeferredTasksExecutor();
    DeferredTasksExecutor(unsigned int tasks);
    ~DeferredTasksExecutor();
    void addTask(DeferredTask *task);
private:
    std::deque<DeferredTask*> tasks;

};

#endif // DEFERREDTASKSEXECUTOR_H
