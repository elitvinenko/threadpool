#include "task.h"

Task::Task()
{
}

void Task::exec()
{
    if (m_fn_ptr)
        (*m_fn_ptr)(m_arg);
}

const int & Task::getId() const
{
    return m_Id;
}

void Task::setId(int Id)
{
    m_Id = Id;
}

