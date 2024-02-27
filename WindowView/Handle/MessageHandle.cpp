#include "MessageHandle.h"

#include <cstring>
#include <iostream>

MessageHandle::MessageHandle(Element* _element):Handle(_element)
{
}

bool MessageHandle::parse(char* _key, char* _payload, char* _res_key, char* _response) const
{
    if(strcmp(_key, "message") == 0)
    {
        std::cout << _payload << std::endl;
        // _memccpy(_response, "get", '\0', 4);
        return true;
    }else
    {
        return Handle::parse(_key, _payload, _res_key, _response);
    }
}
