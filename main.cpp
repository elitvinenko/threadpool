#include <cstdio>
#include <chrono>
#include <iostream>
#include <random>
#include <unistd.h>
#include <time.h>
#include "deferredtasksexecutor.h"
#include "task.h"

void func(void*)
{
    int timePeriod = rand()%1000000;
    std::cout << "sleeping !" << timePeriod << std::endl;
    usleep(timePeriod);
    std::cout << "done" << std::endl;
}

void testWithPrecondition(DeferredTasksExecutor &executor)
{
    std::cout << "ordering test with precondition" << std::endl;
    void (*fn_ptr)(void*) = func;
    Task task(fn_ptr,NULL);
    Task task2(fn_ptr,NULL);
    std::set<int> taskSet;
    int taskId;
    // First of all, we adding 10 tasks with low priority (0-40)
    // and add them to precondition list for next tasks
    for(int i=0;i<10;i++) {
        taskId = executor.addTask(task, random()%40);
        task2.addPrecondition(taskId);
    }
    // Adding 4 theads with high priority (40-80)
    // They have to start after the first ten
    executor.addTask(task2, random()%40+40);
    executor.addTask(task2, random()%40+40);
    executor.addTask(task2, random()%40+40);
    executor.addTask(task2, random()%40+40);
}

void testOrderingByPrioriy(DeferredTasksExecutor &executor)
{
    std::cout << "ordering test " << std::endl;
    void (*fn_ptr)(void*) = func;
    Task task(fn_ptr,NULL);
    for(int i=0;i<30;++i)
        executor.addTask(task,random()%100);
}

int main(int argc, char **argv)
{
    srand (time(NULL));
    DeferredTasksExecutor executor;
    testWithPrecondition(executor);
  //  testOrderingByPrioriy(executor);
    executor.execute();
    executor.wait();

    return 0;
}
