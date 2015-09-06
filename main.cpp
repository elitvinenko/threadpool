#include <cstdio>
#include <chrono>
#include <iostream>
#include <thread>
#include "deferredtasksexecutor.h"
#include "task.h"

void func3(void*)
{
	//string _str = str;
	//this_thread::sleep_for(chrono::milliseconds(2));
    std::cout << "Say hello!" << std::endl;
}

int main(int argc, char **argv)
{
    std::cout << "Hello word!" << std::endl;
    DeferredTasksExecutor executor;
    char c[] = "say hello";
    void (*fn_ptr)(void*) = func3;
    Task task(fn_ptr,NULL);
    executor.addTask(task,3);
    executor.addTask(task,3);
    executor.addTask(task,3);
    executor.addTask(task,3);
    executor.addTask(task,3);
    executor.addTask(task,3);
    executor.addTask(task,3);
    getchar();
    return 0;
}
