#ifndef MY_AUTO_PTR
#define MY_AUTO_PTR


//auto_ptr智能指針
template <typename T>
class AutoPtr
{
    private:
    T* m_ptr;

    public:
    AutoPtr(T* ptr)
    {
        m_ptr = ptr;
    }
    //注意AutoPtr的拷贝构造和赋值运算的形参都不能是const类型，因为这两个函数会改变实参的值m_ptr;
    AutoPtr(const AutoPtr<T>& t)
    {
        m_ptr = t.m_ptr;

        t.m_ptr=0;
    }

    AutoPtr<T>&  operator=( AutoPtr<T> &rhs)
    {
        delete m_ptr;
        m_ptr = rhs.m_ptr;

        rhs.m_ptr=nullptr;

        return *this;
    }

    T& operator*()
    {
        return *m_ptr;
    }
    T* operator->()
    {
        return m_ptr;
    }
    ~AutoPtr()
    {
        if(m_ptr!=nullptr)
        delete m_ptr;
    }

    bool IsNull()
    {
        return m_ptr==nullptr;
    }

    //隐式类型转换操作符：关键字operator之后加上一个类型名称，
    //不能为此函数指定返回类型，因为其返回的值类型已经表现在函数名称上了
    //C++中有两种隐式类型转换：一是自定义的类型中，如果可以通过一个自变量（内置类型的自变量）构造对象或者构造函数的形参中最多只用第一个没有默认值，则可以将相应的内置类型直接转换为该自定义类型
    //例如：
    //class name
    //{
        //public:
        //Name(const string& s);

    //};    //可以将string类型对象直接赋值给Name对象//More　Effective C++条款5  p25
    operator T*()
    {
        return m_ptr;
    }


};





#endif