
// tests.cpp
#include "../deferredtasksexecutor.h"
#include "../task.h"
#include <gtest/gtest.h>
#include <vector>
#include <memory>

void func(void*)
{
    std::cout << "done" << std::endl;
}

TEST(DeferredTaskPoolTest, AddingTasks) {
    DeferredTasksExecutor executor;
    ASSERT_EQ(0, executor.getTasksPriority()->size());
    void (*fn_ptr)(void*) = func;
    Task task(fn_ptr,NULL);
    executor.addTask(task, 40);
    ASSERT_EQ(1, executor.getTasksPriority()->size());
}

TEST(DeferredTaskPoolTest, SortingOrder) {
    DeferredTasksExecutor executor;
    void (*fn_ptr)(void*) = func;
    Task task(fn_ptr,NULL);
    executor.addTask(task, 40);
    executor.addTask(task, 50);
    executor.addTask(task, 00);
    executor.addTask(task, 10);
    executor.addTask(task, 20);
    executor.addTask(task, 60);
    executor.addTask(task, 70);
    executor.addTask(task, 90);
    executor.addTask(task, 30);
    executor.addTask(task, 80);
    std::shared_ptr<std::vector<int>> prioroties = executor.getTasksPriority();
    ASSERT_EQ(90, prioroties->at(0));
    ASSERT_EQ(80, prioroties->at(1));
    ASSERT_EQ(70, prioroties->at(2));
    ASSERT_EQ(60, prioroties->at(3));
    ASSERT_EQ(50, prioroties->at(4));
    ASSERT_EQ(40, prioroties->at(5));
    ASSERT_EQ(30, prioroties->at(6));
    ASSERT_EQ(20, prioroties->at(7));
    ASSERT_EQ(10, prioroties->at(8));
    ASSERT_EQ(00, prioroties->at(9));
}

TEST(DeferredTaskPoolTest, SortingOrderWithDependency) {
    DeferredTasksExecutor executor;
    void (*fn_ptr)(void*) = func;
    Task task(fn_ptr,NULL);
    Task task2(fn_ptr,NULL);
    for (int i=0; i<50; i+=10) {
        int taskId = executor.addTask(task, i);
        task2.addPrecondition(taskId);
    }
    executor.addTask(task2, 70);
    executor.addTask(task2, 80);
    executor.addTask(task2, 60);
    executor.addTask(task2, 90);
    executor.addTask(task2, 50);

    std::shared_ptr<std::vector<int>> prioroties = executor.getTasksPriority();
    ASSERT_EQ(40, prioroties->at(0));
    ASSERT_EQ(30, prioroties->at(1));
    ASSERT_EQ(20, prioroties->at(2));
    ASSERT_EQ(10, prioroties->at(3));
    ASSERT_EQ(00, prioroties->at(4));
    ASSERT_EQ(90, prioroties->at(5));
    ASSERT_EQ(80, prioroties->at(6));
    ASSERT_EQ(70, prioroties->at(7));
    ASSERT_EQ(60, prioroties->at(8));
    ASSERT_EQ(50, prioroties->at(9));
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
