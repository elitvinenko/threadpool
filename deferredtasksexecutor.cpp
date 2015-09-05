#include <thread>
#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <atomic>
#include "deferredtasksexecutor.h"

DeferredTasksExecutor::DeferredTasksExecutor()
{
}

DeferredTasksExecutor::DeferredTasksExecutor(int tasks): m_tasks(tasks)
{
    for(int i=0;i<tasks;++i) {
        std::thread th_worker(worker, (void*)this);
    }
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
        if (tast.isSet()) {
            task();
            {   std::lock_guard<std::mutex> lock(m_worker_mutex);
                for(auto it = m_undoneIterator; it != end(m_tasks); ++it) {
                    if(it->getThread() == threadId) {
                        it->setStatus(DeferredTask::WORKING);
                        it->setThread(threadId);
                        task = it->getTask();
                        break; // TODO: find task with higest priority;
                    }
                }
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
