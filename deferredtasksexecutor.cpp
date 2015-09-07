#include "deferredtasksexecutor.h"
#include "task.h"

#include <thread>
#include <iostream>
#include <utility>
#include <chrono>
#include <memory>


bool Comparer::operator() (const DeferredTask lhs, const DeferredTask rhs)
{
        return (lhs.getPriority() < rhs.getPriority());
}

DeferredTasksExecutor::DeferredTasksExecutor(): DeferredTasksExecutor(std::thread::hardware_concurrency())
{
}

DeferredTasksExecutor::DeferredTasksExecutor(unsigned int taskCount) : m_maxThreads(taskCount)
{
    if (m_maxThreads == 0) 
        m_maxThreads = std::thread::hardware_concurrency();
}

DeferredTasksExecutor::~DeferredTasksExecutor()
{
    terminateWorkers();
}

void DeferredTasksExecutor::execute()
{
    std::cout << "Started with " << m_maxThreads << " threads" << std::endl;
    for(unsigned int i=0;i<m_maxThreads;++i) {
        m_threads.push_back(std::thread(worker,(void*)this));
    }
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

const std::vector<int> & DeferredTasksExecutor::getTasksPriority()
{
    return taskpool.getTasksPriority();
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
