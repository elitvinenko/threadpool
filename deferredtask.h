#ifndef DEFERREDTASK_H
#define DEFERREDTASK_H
#include <thread>
#include "task.h"

class DeferredTask
{
public:
    enum STATUS
    {
        INQEUE,
        WORKING,
        CANCELED,
        DONE
    };

public:
    DeferredTask(const Task & task, const int priority);
    bool cancel();
    STATUS getStatus() const;
    void setStatus(STATUS status);
    void setThread(std::thread::id thread);
    const Task & getTask() const;
    bool isDone();
public:
    int getPriority() const;
    void setPriority(int priority);

private:
    STATUS m_status = INQEUE;
    Task m_task;
    int m_priority;
    void* result;
};

#endif // DEFERREDTASK_H
