#include "task.h"

Task::Task(void (*fn_ptr)(void*), void* arg) : m_fn_ptr(fn_ptr), m_arg(arg) {}

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

void Task::addPrecondition(int precondition)
{
	m_preconditions.insert(precondition);
}

const  std::set<int> & Task::getPreconditions()
{
	return m_preconditions;
}
