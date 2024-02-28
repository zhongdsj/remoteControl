# pragma once

#include <vector>
#include <iostream>
#include <type_traits>
#include <deque>
#include <set>
#include <map>
#include <list>
namespace zdsj
{
    enum my_enum
    {
        UNKNOW = -1,
        INT = 0,
        INT64,
        BOOL,
        CHAR,
        FLOAT,
        DOUBLE,
        CHARPTR,
        STRING,
        VECTOR,
        DEQUE,
        SET,
        MULTISET,
        MAP,
        MULTIMAP,
        LIST,
        CUSTOM
    };
    
    template<typename T,typename... Types>
    struct IsContainerType
    {
        static const bool value = false;
        static const my_enum type = zdsj::CUSTOM;
    };
 
    // Vector类型为true
    template<typename T,typename... Types>
    struct IsContainerType<std::vector<T, Types...>>
    {
        static const bool value = true;
        static const my_enum type = zdsj::VECTOR;
    };
 
    // deque类型
    template<typename T,typename... Types>
    struct IsContainerType<std::deque<T, Types...>>
    {
        static const bool value = true;
        static const my_enum type = zdsj::DEQUE;
    };
 
    // set类型
    template<typename T,typename... Types>
    struct IsContainerType<std::set<T, Types...>>
    {
        static const bool value = true;
        static const my_enum type = zdsj::SET;
    };
 
    // multiset类型
    template<typename T,typename... Types>
    struct IsContainerType<std::multiset<T, Types...>>
    {
        static const bool value = true;
        static const my_enum type = zdsj::MULTISET;
    };
 
    // map类型
    template<typename K,typename V,typename... Types>
    struct IsContainerType<std::map<K, V, Types...>>
    {
        static const bool value = true;
        static const my_enum type = zdsj::MAP;
    };
 
    // multimap类型
    template<typename K,typename V,typename... Types>
    struct IsContainerType<std::multimap<K, V, Types...>>
    {
        static const bool value = true;
        static const my_enum type = zdsj::MULTIMAP;
    };

    // list类型
    template<typename T,typename... Types>
    struct IsContainerType<std::list<T, Types...>>
    {
        static const bool value = true;
        static const my_enum type = zdsj::LIST;
    };
 
    // 定义获取容器类型的模板
    template<typename T,typename... Types>
    constexpr bool is_container_v = IsContainerType<T, Types...>::value;
    
    // 定义获取容器类型的模板
    template<typename T,typename... Types>
    constexpr my_enum is_container_t = IsContainerType<T, Types...>::type;

    // 定义获取容器类型的模板
    template<typename T,typename... Types>
    constexpr bool is_array_v = IsContainerType<T, Types...>::type == zdsj::VECTOR ||
        IsContainerType<T, Types...>::type == zdsj::SET ||
        IsContainerType<T, Types...>::type == zdsj::MULTISET ||
            IsContainerType<T, Types...>::type == zdsj::DEQUE ||
                IsContainerType<T, Types...>::type == zdsj::LIST;

    template<typename T,typename... Types>
    constexpr bool is_KV_v = IsContainerType<T, Types...>::type == zdsj::MAP ||
        IsContainerType<T, Types...>::type == zdsj::MULTIMAP;
    class SerializeAble;
    class Serialize
    {
    public:
        Serialize()
        {
        }

        ~Serialize()
        {
            
        }
        
        Serialize& operator << (const char* data);

        template<class T>
        Serialize& operator << (T data)
        {
            this->write(data);
            this->obj_num++;
            return *this;
        }

        template<class T, class ...Args>
        void write_args(T& data, Args& ...args)
        {
            this->write(data);
            this->write_args(args...);
        }

        void write_args()
        {
            
        }
        
        Serialize& operator >> (char*& data);

        template<class T>
        Serialize& operator >> (T& data)
        {
            bool res = this->read(data);
            if(!res)
            {
                std::cout << "反序列化失败" << std::endl;
            }
            return *this;
        }
        
        template<class T, class ...Args>
        bool read_args(T& data, Args& ...args)
        {
            this->read(data);
            return this->read_args(args...);
        }

        bool read_args()
        {
            return true;
        }

        void load(std::string data)
        {
            size_t size = data.size();
            this->reserve(size);
            memcpy_s(&this->m_buf[0], size, data.c_str(), size);
        }

        std::string toStream()
        {
            int size = this->m_buf.size();
            std::string result;
            result.reserve(size);
            result.resize(size);
            memcpy_s(const_cast<char*>(result.c_str()), size, (&this->m_buf[0]), size);
            return result;
        }

        void show()
        {
            std::cout << "obj_num: " << this->obj_num << ", buffer size: " << this->m_buf.size() << std::endl;
        }

        bool isEmpty()
        {
            return this->m_buf.size() == 0;
        }

        /**
         * 清空数据
         */
        void clear()
        {
            this->m_buf.clear();
            this->m_pos = 0;
            this->obj_num = 0;
        }

        /**
         * 将指针位置移动到起始位置
         */
        void begin()
        {
            this->seek(0);
        }

        /**
         * 移动指针位置
         */
        void seek(int val)
        {
            this->m_pos = val;
        }

        /**
         * 获取指针位置
         */
        int tell()
        {
            return this->m_pos;
        }
    private:
        
        void reserve(int len);

#ifndef WRITE
#define WRITE
        template<class T>
        void writeType(T data)
        {
            auto type_name = this->getTypeEnum(data);
            this->write((char*)&type_name, sizeof(char));
        }
        void write(const char* data, int len);

        // 偏特化
        void write(const char* data);

        // 自定义类型
        void write(SerializeAble& serialize_able);

        // 模版传入过非容器类型，调用容器函数报错
        // 元编程

        // 类list类型
        template<class T>
        std::enable_if_t<is_array_v<T>, void> write(const T& data)
        {
            this->writeType(data);
            int len = data.size();
            this->write(len);
            for(auto iter = data.begin(); iter != data.end(); iter++)
            {
                this->write(*iter);
            }
        }

        // k,v类型
        template<class T>
        std::enable_if_t<is_KV_v<T>, void> write(const T& data)
        {
            this->writeType(data);
            int len = data.size();
            this->write(len);
            for(auto iter = data.begin(); iter != data.end(); iter++)
            {
                this->write(iter->first);
                this->write(iter->second);
            }
        }

        // std::string类型
        template<class T>
        std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>, void> write(T data)
        {
            this->writeType(data);
            this->write(data.c_str());
        }

        // 基本数据类型
        template<class T, class  = std::enable_if_t<std::is_pod_v<std::decay_t<T>>, void>>
        void write(T data)
        {
            this->writeType(data);
            this->write((char*)&data, sizeof(data));
        }
        
        template<class T>
        my_enum getTypeEnum(T data)
        {
            // std::is_same_v == std::is_same::value 判断类型是否相等
            // std::decay_t == std::decay::type 退化类型  去除const 和 引用
            using data_type = std::decay_t<T>;
            if(std::is_same_v<data_type, int32_t>)
            {
                return my_enum(zdsj::INT);
            }
            if(std::is_same_v<data_type, bool>)
            {
                return my_enum(zdsj::BOOL);
            }
            if(std::is_same_v<data_type, char>)
            {
                return my_enum(zdsj::CHAR);
            }
            if(std::is_same_v<data_type, float>)
            {
                return my_enum(zdsj::FLOAT);
            }
            if(std::is_same_v<data_type, double>)
            {
                return my_enum(zdsj::DOUBLE);
            }
            if(std::is_same_v<data_type, int64_t>)
            {
                return my_enum(zdsj::INT64);
            }
            if(std::is_same_v<data_type, std::string>)
            {
                return my_enum(zdsj::STRING);
            }
            return my_enum(is_container_t<data_type>);
        }
#endif
        
#ifndef READ
#define READ

        void read(char*& data);

        // 自定义类型
        bool read(SerializeAble& serialize_able);
        
        // 类list类型
        template<class T>
        std::enable_if_t<is_array_v<T>, bool> read(T& data)
        {
            auto read_type = my_enum(this->m_buf[this->m_pos]);
            this->m_pos++;
            if(this->checkType(data, read_type))
            {
                int len;
                this->read(len);
                typename T::value_type temp;
                for(int i = 0; i < len; ++i)
                {
                    this->read(temp);
                    data.insert(data.end(), temp);
                }
                return true;
            }else
            {
                return false;
            }
        }

        // k,v类型
        template<class T>
        std::enable_if_t<is_KV_v<T>, bool> read(T& data)
        {
            auto read_type = my_enum(this->m_buf[this->m_pos]);
            this->m_pos++;
            if(this->checkType(data, read_type))
            {
                int len;
                this->read(len);
                typename T::key_type key;
                typename T::mapped_type value;
                for(int i = 0; i < len; ++i)
                {
                    this->read(key);
                    this->read(value);
                    data.insert(typename T::value_type(key, value));
                }
                return true;
            }else
            {
                return false;
            }
        }

        // std::string类型
        template<class T>
        std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>, bool> read(T& data)
        {
            auto read_type = my_enum(this->m_buf[this->m_pos]);
            this->m_pos++;
            if(this->checkType(data, read_type))
            {
                char* temp;
                try
                {
                    this->read(temp);
                    data = std::string(temp);
                    delete[] temp;
                    return true;
                }
                catch (...)
                {
                    return false;
                }
            }else
            {
                return false;
            }
        }

        // 基本数据类型
        template<class T, class = std::enable_if_t<std::is_pod_v<std::decay_t<T>>, void>>
        bool read(T& data)
        {
            auto read_type = my_enum(this->m_buf[this->m_pos]);
            this->m_pos++;
            if(this->checkType(data, read_type))
            {
                data = *(T*)(&this->m_buf[this->m_pos]);
                this->m_pos += sizeof(T);
                return true;
            }else
            {
                return false;
            }
        }
        

        template<class T>
        bool checkType(T& data, my_enum read_type)
        {
            using type_name = std::decay_t<T>;
            return (read_type == this->getTypeEnum(data));
        }
#endif
        
        std::vector<char> m_buf;
        int m_pos = 0;
        int obj_num = 0;
    };

    class SerializeAble
    {
    public:
        virtual void serialize(Serialize& stream) const = 0;
        virtual bool unSerialize(Serialize& stream) = 0;
    };

    #define SERIALIZEABLE(...)                                  \
        void serialize(zdsj::Serialize& stream) const override  \
        {                                                       \
            stream.write_args(__VA_ARGS__);                     \
        }                                                       \
        bool unSerialize(zdsj::Serialize& stream) override      \
        {                                                       \
            return stream.read_args(__VA_ARGS__);               \
        }                                                       \

    
}

