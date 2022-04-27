#ifndef MY_STL_STRING_H
#define MY_STL_STRING_H

#include "exceptdef.h"
#include <cstring>

#define STRING_BUFFER 32
class String
{

public:
    class CharProxy
    {
        private:
        String& curString;
        int curIndex;
        public:
        CharProxy(String& thisString,int index);

        CharProxy& operator=(const CharProxy& rhs);
        CharProxy& operator=(char c);
        operator char() const;
        private:
        void copy_on_write();
    };


private:
    char* m_pData;
    int*  m_pRefCount;

    size_t m_size;
    size_t m_cap;
public:
    String(const char* ipData = "");

    String& operator=(const String &str);
    String(const String &str);
    String(String &&rhs);

    ~String();

    const char* c_str();

    size_t size()
    {
        return m_size;
    }

    void reserve(size_t newCap)
    {
        if(newCap<m_cap)
        return;
        else
        {
            char* ori = m_pData;
            m_pData = new char[newCap+1];
            //char* p = strcpy(m_pData,ori);
            if(--*m_pRefCount==0)
            {
                delete []ori;
                delete m_pRefCount;
            }
            m_pRefCount = new int(1);
        }
    }

    CharProxy operator[](int index);
    const CharProxy operator[](int index) const;

    //用于测试的时候取地址
    const char* begin()const
    {
        return m_pData;
    }

private:
    void copy_from(char* ipSrc,size_t count);

    friend class CharProxy;

};








#endif