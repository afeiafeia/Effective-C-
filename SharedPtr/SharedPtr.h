#ifndef MY_SHARED_PTR
#define MY_SHARED_PTR

#include <type_traits>
#include "type_traits.h"

template<class T>
class SharedPtr
{
    typedef T                   value_type;
    typedef T*                  pointer;
    typedef T&                  reference;

private:
    int *m_use;  //引用计数，注意要用动态内存
    T* m_ptr;


public:
    //static T count;
public:

    SharedPtr()
    :m_use(nullptr)
    ,m_ptr(nullptr)
    {
    }
    //构造
    SharedPtr(T* ptr)
    :m_use(new int(1))
    ,m_ptr(ptr)
    {
        //count++;
    }

    SharedPtr(const SharedPtr &t)
    :m_ptr(t.m_ptr)
    ,m_use(t.m_use)
    {
        Increase();
    }

    //SharedPtr(SharedPtr&& rhs)
    //:m_ptr(rhs.m_ptr)
    //,m_use(rhs.m_use)
    //{
    //    rhs.m_ptr = 0;
    //    rhs.m_use = 0;
    //    int s=0;
    //    int ss=2;
    //}
    //赋值
    SharedPtr& operator=(const SharedPtr &rhs)
    {
        Decrease();
        m_ptr = rhs.m_ptr;
        m_use = rhs.m_use;
        Increase();
        return *this;
    }

   SharedPtr& operator=(const T* ptr)
   {
      Assign(const_cast<T*>(ptr));
      return *this;

   }

    //SharedPtr& operator=(SharedPtr&& rhs)
    //{
    //    //std::swap(*this,rhs);
    //    Decrease();
    //    this->m_ptr = rhs.m_ptr;
    //    this->m_use = rhs.m_use;
    //    rhs.m_ptr = 0;
    //    rhs.m_use=0;
    //    return *this;
    //}

    template<class T2>
    bool operator==(const SharedPtr<T2>& rhs)const
    {
        return m_ptr=rhs.get();
    }

    //智能指针只能放在==号的左边
    template<class T2>
    bool operator==(const T2* ptr)const
    {
        return m_ptr==ptr;
    }

    //智能指针可以放在==右边
    template<class T2>
    friend bool operator==(const T2* left,const SharedPtr&right)
    {
        return left == right.get();
    }

    template<class T2>
    bool operator!=(const SharedPtr<T2>& rhs)const
    {
        return get()!=rhs.get();
    }


    template<class T2>
    bool operator!=(const T2* ptr)const 
    {
        return get()!=ptr;
    }

    template<class T2>
    friend bool operator!=(const T2* left,const SharedPtr& right)
    {
        return left!=right.get();
    }







    //析构
    ~SharedPtr()
    {
        Decrease();
    }

    //解引用
    T& operator*()
    {
        return *get();
    }
    T* operator->()
    {
        return m_ptr;
    }

    T* get() const
    {
        return static_cast<T*>(this->m_ptr);
    }

    const int* getCount() const
    {
        return m_use;
    }

    //判断是否为空

    void Nullify()
    {
        Decrease();
    }

    bool IsNull() const 
    {
        return m_ptr==0;
    }
    //隐式类型转换，可以直接将智能指针赋给指针;例如一般赋值或者传递参数（传递参数实际上是拷贝构造），也可以用来判断是否为空
   operator T*()
   {
       return m_ptr;
   }




    //与派生类、基类的转换（实现真实指针的一些特点）
    //派生类到基类的转换、基类到派生类的转换
    //由于SharedPtr是模板，实例化之后，原来具有继承关系的类型的指针就不能直接转换了
    //方式1：
    //使用一个成员函数模板,但是这种方式将使得基类指针转成派生类指针（这个是不应该的）
    //template<class newType>
    //operator SharedPtr<newType>()
    //{
    //    return SharedPtr<newType>(static_cast<newType*>(m_ptr));
    //    //return SharedPtr<newType>(static_cast<newType*>(static_cast<void*>(m_ptr)));
    //}


    //template<class T2>
    //operator SharedPtr<typename mystl::enable_if<std::is_base_of<T,T2>::value,T2>::value_type>()
    //{
    //    //return SharedPtr<newType>(static_cast<newType*>(m_ptr));
    //    return SharedPtr<T2>(static_cast<T2*>(static_cast<void*>(m_ptr)));
    //}

    //方式二：定义一个静态函数(不好的方式)
    //template<class T2>
    //static typename std::enable_if<is_base_but_not_same<T2,T>>::value
    //DownCast(const SharedPtr(T2) &theObj)
    //{
        //return SharedPtr(dynamic_cast<T*>(const_cast<T2*>(theObj.m_ptr)));
    //}

    //template <class T2>
    //static SharedPtr
    //DownCast(const SharedPtr<T2> &theObj)
    //{
    //
    //    //return SharedPtr<dynamic_cast<T*>(const_cast<T2*>(theObj.m_ptr))>;
    //}

    //方式二：定义一个静态函数（利用萃取）
    //向上转型
    //基类智能指针转换成派生类
    template<class T2>
    static typename mystl::enable_if<std::is_base_of<T2,T>::value,SharedPtr>::value_type
    DownCast(SharedPtr<T2>& rhs)
    {
        //注意引用计数
       SharedPtr tmp = SharedPtr(dynamic_cast<T*>(const_cast<T2*>(rhs.get())));
       tmp.m_use = const_cast<int*>(rhs.getCount());
       (*tmp.m_use)++;
       return tmp;
    }


    //基类指针转换成派生类智能指针
    template<class T2>
    static typename mystl::enable_if<std::is_base_of<T2,T>::value,SharedPtr>::value_type
    DownCast(T2* rhs)
    {
        return SharedPtr(dynamic_cast<T*>(const_cast<T2*>(rhs)));
    }


    //派生类转换成基类
    template<class T2>
    static typename mystl::enable_if<std::is_base_of<T,T2>::value,SharedPtr>::value_type
    UpCast(const SharedPtr<T2>& rhs)
    {
        SharedPtr tmp = SharedPtr(dynamic_cast<T*>(const_cast<T2*>(rhs.get())));
         tmp.m_use = const_cast<int*>(rhs.getCount());
        (*tmp.m_use)++;
        return tmp;
    }

    template<class T2,typename = typename mystl::enable_if<std::is_base_of<T2,T>::value,SharedPtr>::value_type>
    operator SharedPtr<T2>& ()
    {
        //return SharedPtr(static_cast<T*>(this->get()));
        return reinterpret_cast<SharedPtr<T2>&>(*this);
    }

    //向上转型与向下转型的不同在于：
    //向上转型要考虑是否可以进行（存在虚函数且基类指针指向派生类对象，转型才有意义，因此使用dynamic_cast）,
    //另外要考虑的问题是：转型的过程是重构了一个派生类的智能指针，与基类智能指针指向同一个对象，但二者的引用计数指针不一致，因此要进行调整

    //而对于向下转型，由于派生类指针从语法上就可以直接转成基类指针，因此，可以直接定义一个转换符operator，但是要注意，不能让其他不具有继承关系的类型的指针也转换，因此要进行萃取
    //另外，可以不进行基类智能指针的重构，直接转换成引用即可




private:

    void Assign(T* ptr)
    {
        if(m_ptr == ptr)
        return;
        Decrease();
        m_ptr = ptr;
        Increase();
    }
    void Decrease()
    {
        if(m_use&&--*m_use==0)
        {
            delete m_ptr;
            delete m_use;

             m_ptr = nullptr;
            m_use = nullptr;
        }

    }

    void Increase()
    {
        if(m_ptr)
        {
            ++*m_use;
        }
    }

    
    };//SharedPtr

    //以上这种方式不能实现指针那种const的转换： int *p = new int(3); const int* pp = p; const int* p3 = new int(3);
    //int* 对应智能指针：SharedPtr<int>;
    //const int* 对应智能指针类：SharedPtr<const int>
    //但它们是两种完全不同的类型，因此不能直接转换：
    //SharedPtr<int> ptr = new int(1); 
    //SharedPtr<const int> constPtr = ptr;  //不正确
    
    //如果要实现上述转换，可以使用一个SharedConstPtr模板类作为基类，使得SharedPtr继承自SharedConstPtr






#endif//MY_SHARED_PTR