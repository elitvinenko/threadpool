#ifndef TASK_H
#define TASK_H
#include <set>

class Task
{
public:
    Task(void (*fn_ptr)(void*), void* arg);
    void exec();
    const int & getId() const;
    void setId(int Id);
    void addPrecondition(int precondition);
    const std::set<int> & getPreconditions();

private:
    std::set<int> m_preconditions = {};
    void (*m_fn_ptr)(void*) = nullptr;
    void* m_arg;
    int m_Id;
};

#endif // TASK_H
