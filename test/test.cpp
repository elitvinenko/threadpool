
// tests.cpp
#include "../deferredtasksexecutor.h"
#include "../task.h"
#include <gtest/gtest.h>
#include <vector>

void func(void*)
{
    std::cout << "done" << std::endl;
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
    std::vector<int> prioroties = executor.getTasksPriority();
    ASSERT_EQ(10, prioroties.size());
    ASSERT_EQ(90, prioroties[0]);
    ASSERT_EQ(80, prioroties[1]);
    ASSERT_EQ(70, prioroties[2]);
    ASSERT_EQ(60, prioroties[3]);
    ASSERT_EQ(50, prioroties[4]);
    ASSERT_EQ(40, prioroties[5]);
    ASSERT_EQ(30, prioroties[6]);
    ASSERT_EQ(20, prioroties[7]);
    ASSERT_EQ(10, prioroties[8]);
    ASSERT_EQ(00, prioroties[9]);
}

//TEST(SquareRootTest, NegativeNos) {
//    ASSERT_EQ(-1.0, squareRoot(-15.0));
//    ASSERT_EQ(-1.0, squareRoot(-0.2));
//}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
