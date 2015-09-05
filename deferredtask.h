#ifndef DEFERREDTASK_H
#define DEFERREDTASK_H
#include <thread>

class Task
{
public:
    void (*m_fn_ptr)(void*) = nullptr;
    void* m_arg;
    void operator()()
    {
        if (m_fn_ptr)
            (*m_fn_ptr)(m_arg);
    }
    bool isSet()
    {
        return m_fn_ptr != nullptr;
    }
};

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
    const STATUS & getStatus()
    {
        return m_status;
    }
    void setStatus(STATUS status)
    {
        m_status = status;
    }
    const std::thread::id & getThread() const
    {
        return m_workingThreadId;
    }
    void setThread(std::thread::id thread)
    {
        m_workingThreadId = thread;
    }
    const Task & getTask() const
    {
        return m_task;
    }

    bool isDone()
    {
        return (m_status == DONE || m_status == CANCELED);
    }


private:
    int priority;
    std::thread::id m_workingThreadId;
    STATUS m_status = INQEUE;
    Task m_task;\
};

#endif // DEFERREDTASK_H
