#ifndef TASK_H
#define TASK_H

class Task
{
public:
    Task(void (*fn_ptr)(void*), void* arg);
    void exec();
    bool isSet()
    {
        return m_fn_ptr != nullptr;
    }
    const int & getId() const;
    void setId(int Id);

private:
    void (*m_fn_ptr)(void*) = nullptr;
    void* m_arg;
    int m_Id;
};

#endif // TASK_H
