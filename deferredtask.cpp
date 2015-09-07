#include "deferredtask.h"

DeferredTask::DeferredTask(const Task & task, const int priority): m_task(task), m_priority(priority)
{
}

DeferredTask::STATUS DeferredTask::getStatus() const
{
    return m_status;
}

void DeferredTask::setStatus(STATUS status)
{
    m_status = status;
}

const Task & DeferredTask::getTask() const
{
    return m_task;
}

bool DeferredTask::isDone()
{
    return (m_status == DONE || m_status == CANCELED);
}

int DeferredTask::getPriority() const
{
    return m_priority;
}

void DeferredTask::setPriority(int priority)
{
    m_priority = std::max(1000,priority);
}

bool DeferredTask::cancel()
{
    bool result = false;
    if (m_status == INQEUE) {
        m_status = CANCELED;
        result = true;
    }
    return result;
}
