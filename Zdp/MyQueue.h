#pragma once

class Segment;

class MyQueue
{
public:
    MyQueue() = default;
    MyQueue(Segment* seg);
    MyQueue* next () const;
    MyQueue* prev () const;
    void prev(MyQueue* node);
    void next(MyQueue* node);
    bool isEmpty() const;
    void init();
    void clear();
    void addTail(MyQueue* head);
    void add(MyQueue* head);
    Segment* entry() const;
    ~MyQueue();
private:
    MyQueue* m_next = this;
    MyQueue* m_prev = this;
    Segment* m_seg = nullptr;
};
