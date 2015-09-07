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

int DeferredTasksExecutor::addTask(Task & task, int priority)
{
    return taskpool.addTask(task, priority);
}

bool DeferredTasksExecutor::cancelTask(int taskId)
{
   return taskpool.cancelTask(taskId);
}

void DeferredTasksExecutor::realWorker()
{
    std::thread::id threadId = std::this_thread::get_id();
    std::cout << "Starting worker..." << threadId <<  std::endl;
    while (true) {
        Task *task = nullptr;
        if (( task = taskpool.getTask() )) {
            task->exec();
            taskpool.setDoneStatus(task->getId());
            delete task;
        }
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
