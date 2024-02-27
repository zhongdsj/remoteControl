#include "Handle.h"

Handle::Handle(Element* element): m_element(element)
{
    
}

Handle::~Handle()
{
    delete this->m_next;
}

void Handle::add(Handle* _nxt)
{
    this->m_next = _nxt;
}

bool Handle::parse(char* _key, char* _payload, char* _res_key, char* _response) const
{
    if(this->m_next != nullptr)
    {
        return this->m_next->parse(_key, _payload, _res_key, _response);
    }
    return false;
}
