# include "Serialize.h"


namespace zdsj
{
    Serialize& Serialize::operator<<(const char* data)
    {
        auto type_name = zdsj::CHARPTR;
        this->write((char*)&type_name, sizeof(char));
        this->write(data);
        this->obj_num++;
        return *this;
    }

    Serialize& Serialize::operator>>(char*& data)
    {
        this->m_pos++;
        this->read(data);
        return *this;
    }

    void Serialize::write(const char* data, int len)
    {
        int size = this->m_buf.size();
        this->reserve(len);
        memcpy_s(&this->m_buf[size], size+len, data, len);
    }

    void Serialize::reserve(int len)
    {
        // 当前存储大小
        int size = this->m_buf.size();

        // 可存储大小
        int cap = this->m_buf.capacity();
        while (cap < size + len)
        {
            if (cap == 0)
            {
                cap = 1;
            }else
            {
                cap *= 2;
            }
            this->m_buf.reserve(cap);
        }
        this->m_buf.resize(size + len);
    }

    void Serialize::write(const char* data)
    {
        int len = strlen(data) + 1;
        this->write(len);
        this->write(data, len);
    }

    void Serialize::write(SerializeAble& serialize_able)
    {
        auto type_name = zdsj::CUSTOM;
        this->write((char*)&type_name, sizeof(char));
        serialize_able.serialize(*this);
    }

    void Serialize::read(char*& data)
    {
        int len;
        this->read(len);
        char* result = new char[len];
        memcpy_s(result, len, &this->m_buf[this->m_pos], len);
        data = result;
        this->m_pos += len;
    }

    bool Serialize::read(SerializeAble& serialize_able)
    {
        auto read_type = my_enum(this->m_buf[this->m_pos]);
        this->m_pos++;
        if(read_type == zdsj::CUSTOM)
        {
            return serialize_able.unSerialize(*this);
        }else
        {
            return false;
        }
    }
}
