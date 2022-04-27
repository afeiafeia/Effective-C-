#include "String.h"
#include "exceptdef.h"
#include <iostream>
#include <stdio.h>

String::CharProxy::CharProxy(String& thisString,int index)
:curString(thisString)
,curIndex(index)
{

}
String::CharProxy& String::CharProxy::operator=(const CharProxy& rhs)
{
    //写时复制 
    copy_on_write();
    curString.m_pData[curIndex] = rhs.curString.m_pData[rhs.curIndex];
    return *this;

}

String::CharProxy& String::CharProxy::operator=(char c)
{
    copy_on_write();
    curString.m_pData[curIndex] = c;
    return *this;
}

void  String::CharProxy::copy_on_write()
{
    char* tmpData = curString.m_pData;
    int cap = curString.m_cap;
    curString.m_pData=new char[cap];
    strcpy(curString.m_pData,tmpData);
    if(--curString.m_pRefCount==0)
    {
        delete []tmpData;
        delete curString.m_pRefCount;
    }
    curString.m_pRefCount = new int(1);
    
}





String::String(const char* ipData)
:m_pData(const_cast<char*>(ipData))
,m_pRefCount(new int(1))
{
    //传入的是字符串，这是是必须要复制一份的，而不能只用指针指向同一字符串
    //因为外面的字符串不是string对象，如果在外面将其释放掉，这个string是无法知道的
    //string之间拷贝、复制才考虑写时复制
    size_t arrLen = strlen(ipData);
    m_size = arrLen;
    m_cap = STRING_BUFFER>arrLen+1?STRING_BUFFER:arrLen+1;
    m_pData = new char[m_cap];
    //strcpy拷贝'\0'之前的所有字符
    strcpy(m_pData,ipData);
}

String& String::operator=(const String &str)
{
    if(--*m_pRefCount==0)
    {
        delete []m_pData;
        delete m_pRefCount;
    }

    //浅拷贝，写时再复制
    m_pData = str.m_pData;
    m_size = str.m_size;
    m_cap = str.m_cap;

    m_pRefCount = str.m_pRefCount; 
    ++*m_pRefCount;
}
String::String(const String& rhs)
:m_pData(rhs.m_pData)
,m_pRefCount(rhs.m_pRefCount)
,m_size(rhs.m_size)
,m_cap(rhs.m_cap)
{
    //浅拷贝，写时再复制
    ++*m_pRefCount;
}
String::String(String &&rhs)
:m_pData(rhs.m_pData)
,m_pRefCount(rhs.m_pRefCount)
,m_size(rhs.m_size)
,m_cap(rhs.m_cap)
{
    rhs.m_pData = nullptr;
    rhs.m_pRefCount = nullptr;

}

String::~String()
{
    if(--*m_pRefCount==0)
    {
        delete []m_pData;
        m_pData = nullptr;

        delete m_pRefCount;
        m_pRefCount = nullptr;
    }
}


void String::copy_from(char* ipSrc,size_t count)
{
    THROW_OUT_OF_RANGE_IF(strlen(ipSrc)<count,"拷贝范围太大！");
    strcpy(m_pData,ipSrc);
}

String::CharProxy String::operator[](int index)
{
    return CharProxy(*this,index);
}

String::CharProxy::operator char()const
{
    return curString.m_pData[curIndex];
}
