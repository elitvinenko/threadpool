#ifndef DEFERREDTASKPOOL_H
#define DEFERREDTASKPOOL_H
#include <queue>
#include <list>

template< typename T>
class DeferredTaskPool
{
public:
    DeferredTaskPool();
    void addTask();
    void doneTask();
protected:
    std::list<T> m_undoneTasks;
    std::list<T> m_doneTasks;
};

#endif // DEFERREDTASKPOOL_H
