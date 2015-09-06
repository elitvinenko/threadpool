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
    int getId() const;
    const std::thread::id & getThread() const;
    void setStatus(STATUS status);
    void setThread(std::thread::id thread);
    const Task & getTask() const;
    bool isDone();
    int getPriority() const;
    void setPriority(int priority);

private:
    std::thread::id m_workingThreadId;
    STATUS m_status = INQEUE;
    Task m_task;\
    int m_priority;
};

#endif // DEFERREDTASK_H
