#include <cstdio>
#include <chrono>
#include <iostream>
#include <thread>
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

int main(int argc, char **argv)
{
    srand (time(NULL));
    DeferredTasksExecutor executor();
    void (*fn_ptr)(void*) = func;
    Task task(fn_ptr,NULL);
    for(int i=0;i<100;++i)
        executor.addTask(task,random()%100);
    executor.addTask(task,random()%100);
    executor.addTask(task,random()%100);
    executor.addTask(task,random()%100);
    getchar();
    return 0;
}
