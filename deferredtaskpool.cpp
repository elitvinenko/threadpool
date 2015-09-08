#include "deferredtaskpool.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <set>

DeferredTaskPool::DeferredTaskPool()
{
}

int DeferredTaskPool::addTask(Task & _task, int priority)
{
    _task.setId(++m_lastTaskId);
    DeferredTask deferredTask(_task, priority);
    {
        std::lock_guard<std::mutex> lock(m_worker_mutex);
        Pool::iterator itSkipped = m_undoneIterator;
        // Just skip all depended values
        if (!_task.getPreconditions().empty()) {
            for(Pool::iterator it = m_undoneIterator; it != end(m_tasks); ++it) {
                if(_task.getPreconditions().count(it->getTask().getId())) {
                    itSkipped = it;
                }
            }
            if (itSkipped != m_tasks.end())
                ++itSkipped;
        }
        // try to find process place based on process priority
        bool isFound = false;
        for(; itSkipped != m_tasks.end(); ++itSkipped) {
            if (!itSkipped->isDone())
                if( itSkipped->getPriority() < priority) {
                    isFound = true;
                    break;
                }
        }
        if (isFound)
            m_tasks.insert(itSkipped, deferredTask);
        else
            m_tasks.push_back(deferredTask);
        m_undoneIterator = begin(m_tasks);
        for(;m_undoneIterator != end(m_tasks) && m_undoneIterator->isDone(); ++m_undoneIterator);
    }
    return deferredTask.getTask().getId();
}

void DeferredTaskPool::printfPoll()
{
    std::cout << "Pool content:" << std::endl;
    for(auto task:m_tasks) {
        int prio = task.getPriority();
        std::cout << "task prio:" << prio  << std::endl;
    }
}

// without mutex!!!
std::shared_ptr<std::vector<int>> DeferredTaskPool::getTasksPriority()
{
    std::vector<int> res;
    for(auto task:m_tasks) {
        res.push_back(task.getPriority());
    }
    return std::make_shared<std::vector<int>>(res);
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

bool DeferredTaskPool::hasTasks()
{
    std::lock_guard<std::mutex> lock(m_worker_mutex);
    if(m_undoneIterator != m_tasks.end()) {   // We have tasks in queue
        for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
            if(it->getStatus() == DeferredTask::INQEUE || it->getStatus() == DeferredTask::WORKING ) {
                return true;
            }
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
