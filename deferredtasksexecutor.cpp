#include <thread>
#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <atomic>
#include <memory>
#include <boost/heap/priority_queue.hpp>
#include "deferredtasksexecutor.h"
#include "task.h"


bool Comparer::operator() (const DeferredTask lhs, const DeferredTask rhs)
{
        return (lhs.getPriority() < rhs.getPriority());
}

DeferredTasksExecutor::DeferredTasksExecutor(): DeferredTasksExecutor(std::thread::hardware_concurrency())
{
}

DeferredTasksExecutor::DeferredTasksExecutor(int taskCount)
{
    std::cout << "Started with " << taskCount << " threads" << std::endl;
    for(int i=0;i<taskCount;++i) {
        m_threads.push_back(std::thread(worker,(void*)this));
    }
}

DeferredTasksExecutor::~DeferredTasksExecutor()
{
    terminateWorkers();
}

void DeferredTasksExecutor::terminateWorkers()
{
    m_terminate = true;
    // wait for closing all threads
    for(std::thread & worker:m_threads) {
        if(worker.joinable())
            worker.join();
    }
}

int DeferredTasksExecutor::addTask(Task & _task, int priority)
{
    _task.setId(++m_lastTaskId);
    DeferredTask task(_task, priority);
    {
        std::lock_guard<std::mutex> lock(m_worker_mutex);
        DeferredTaskPool::iterator it;
        for(it = m_undoneIterator; it < end(m_tasks); ++it) {
            if (!it->isDone())
                if(it->getPriority()<priority)
                    break;
        }
        m_tasks.insert(it, task);
        m_undoneIterator = begin(m_tasks);
        for(;m_undoneIterator != end(m_tasks) && m_undoneIterator->isDone(); ++m_undoneIterator);
    }
    return task.getId();
}

bool DeferredTasksExecutor::cancelTask(int taskId)
{
    std::lock_guard<std::mutex> lock(m_worker_mutex);
    for(auto & task:m_tasks) {
        if(task.getId() == taskId) {
            return task.cancel();
        }
    }
    return false;
}

void DeferredTasksExecutor::realWorker()
{
    std::thread::id threadId = std::this_thread::get_id();
    std::cout << "Starting worker..." << threadId <<  std::endl;
    while (true) {
        Task *task = nullptr; //TODO: move task to unique_ptr
        {   std::lock_guard<std::mutex> lock(m_worker_mutex);
            if(m_undoneIterator != m_tasks.end()) {   // We have tasks in queue
                for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
                    if(it->getStatus() == DeferredTask::INQEUE) {
                        it->setStatus(DeferredTask::WORKING);
                        it->setThread(threadId);
                        task = new Task(it->getTask());
                        break;
                    }
                }
            }
        }
        if (task) {
            std::cout << "new task from " << threadId << std::endl;
            task->exec();
            delete task;
        }
        {   std::lock_guard<std::mutex> lock(m_worker_mutex);
            for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
                if(it->getThread() == threadId) {
                    it->setStatus(DeferredTask::DONE);
                }
            }
            for(m_undoneIterator = begin(m_tasks);m_undoneIterator != end(m_tasks) && m_undoneIterator->isDone(); ++m_undoneIterator);
        }
        //  std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (m_terminate) break;
        std::this_thread::yield();
    }
}

void DeferredTasksExecutor::worker(void * th)
{
    if (th) {
        DeferredTasksExecutor *that =reinterpret_cast<DeferredTasksExecutor*>(th);
        that->realWorker();
    }
}
