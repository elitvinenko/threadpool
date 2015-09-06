#include <thread>
#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <atomic>
#include <memory>
#include "deferredtasksexecutor.h"
#include "task.h"

DeferredTasksExecutor::DeferredTasksExecutor()
{
}

DeferredTasksExecutor::DeferredTasksExecutor(int taskCount)
{
    for(int i=0;i<taskCount;++i) {
        m_threads.push_back(std::thread(worker,(void*)this));
    }
}

int DeferredTasksExecutor::addTask(Task & _task, int priority)
{
    _task.setId(++m_lastTaskId);
    DeferredTask task(_task, priority);
    {
        std::lock_guard<std::mutex> lock(m_worker_mutex);
        m_tasks.push_back(task);
    }
    return m_lastTaskId;
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
    std::cout << "Starting worker..." << std::endl;
    while (true) {
        Task task;
        {   std::lock_guard<std::mutex> lock(m_worker_mutex);
            if(m_undoneIterator != m_tasks.end()) {   // We have tasks in queue
                for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
                    if(it->getStatus() == DeferredTask::INQEUE) {
                        it->setStatus(DeferredTask::WORKING);
                        it->setThread(threadId);
                        task = it->getTask();
                        break;
                    }
                }
            }
        }
//        if (task->isSet()) {
//            task->exec();
            {
            {   std::lock_guard<std::mutex> lock(m_worker_mutex);
                for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
                    if(it->getThread() == threadId) {
                        it->setStatus(DeferredTask::DONE);
                    }
                }
                // Move m_firstUndoneTaskIterator to last done position
                for(;m_undoneIterator != end(m_tasks) && m_undoneIterator->isDone(); ++m_undoneIterator);
            }
        }
      //  std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
