#pragma once

class Element;

class Handle
{
public:
    Handle() = default;
    Handle(Element* element);
    virtual  ~Handle();
    void add(Handle* _nxt);
    virtual bool parse(char* _key, char* _payload, char* _res_key, char* _response) const;
private:
    Element* m_element = nullptr;
    Handle* m_next = nullptr;
};
