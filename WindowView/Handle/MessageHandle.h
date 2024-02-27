#pragma once
#include "Handle.h"

class MessageHandle : public Handle
{
public:
    MessageHandle() = default;
    MessageHandle(Element* _element);
    bool parse(char* _key, char* _payload, char* _res_key, char* _response) const override;
};
