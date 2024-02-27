#include "MyQueue.h"
#include "Segment.h"

MyQueue::MyQueue(Segment* seg):m_seg(seg)
{
    
}

MyQueue*  MyQueue::next() const
{
    return this->m_next;
}

MyQueue* MyQueue::prev() const
{
    return this->m_prev;
}

void MyQueue::prev(MyQueue* node)
{
    this->m_prev = node;
}

void MyQueue::next(MyQueue* node)
{
    this->m_next = node;
}

bool MyQueue::isEmpty() const
{
    return this->m_next == this->m_prev;
}

void MyQueue::init()
{
    this->m_next = this;
    this->m_prev = this;
}

void MyQueue::clear()
{
    this->m_next->m_prev = this->m_prev;
    this->m_prev->m_next = this->m_next;
}

void MyQueue::addTail(MyQueue* head)
{
    this->prev(head->prev());
    this->next(head);
    head->prev()->next(this);
    head->prev(this);
}

void MyQueue::add(MyQueue* head)
{
    this->prev(head);
    this->next(head->next());
    head->next()->prev(this);
    head->next(this);
}

Segment* MyQueue::entry() const
{
    return this->m_seg;
}

MyQueue::~MyQueue()
{
    this->clear();
}
