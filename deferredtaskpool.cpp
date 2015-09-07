#include "deferredtaskpool.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <functional>
#include <atomic>
#include <thread>
#include <mutex>
#include <set>

DeferredTaskPool::DeferredTaskPool()
{
}

int DeferredTaskPool::addTask(Task & _task, int priority)
{
    _task.setId(++m_lastTaskId);
    DeferredTask task(_task, priority);
    {
        std::lock_guard<std::mutex> lock(m_worker_mutex);
        Pool::iterator it;
        // try to find process place based on process priority 
        for(it = m_undoneIterator; it != end(m_tasks); ++it) {
            if (!it->isDone())
                if(it->getPriority()<priority)
                    break;
        }
        // if is preconditions exists, move tast to last precondition
        if (!_task.getPreconditions().empty()) {
            std::set<int> preconditions = _task.getPreconditions();
            for(; it != end(m_tasks) && !preconditions.empty(); ++it) {
                    preconditions.erase(it->getTask().getId());
            }
        }
        std::cout << "Adding task with priority " << priority << std::endl;
        m_tasks.insert(it, task);
        m_undoneIterator = begin(m_tasks);
        for(;m_undoneIterator != end(m_tasks) && m_undoneIterator->isDone(); ++m_undoneIterator);
    }
    return task.getTask().getId();
}

bool DeferredTaskPool::cancelTask(int taskId)
{
    std::lock_guard<std::mutex> lock(m_worker_mutex);
    for(auto & task:m_tasks) {
        if(task.getTask().getId() == taskId) {
            return task.cancel();
        }
    }
    return false;
}

Task *DeferredTaskPool::getTask()
{
    std::lock_guard<std::mutex> lock(m_worker_mutex);
    if(m_undoneIterator != m_tasks.end()) {   // We have tasks in queue
        for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
            if(it->getStatus() == DeferredTask::INQEUE) {
                it->setStatus(DeferredTask::WORKING);
                std::cout << "getting task with priority " << it->getPriority() << std::endl;
                return new Task(it->getTask());
            }
        }
    }
    return nullptr;
}

bool DeferredTaskPool::setDoneStatus(int taskId)
{
    std::lock_guard<std::mutex> lock(m_worker_mutex);
    for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
        if(it->getTask().getId() == taskId) {
            it->setStatus(DeferredTask::DONE);
        }
    }
    for(m_undoneIterator = begin(m_tasks);m_undoneIterator != end(m_tasks) && m_undoneIterator->isDone(); ++m_undoneIterator);
    return true;
}
