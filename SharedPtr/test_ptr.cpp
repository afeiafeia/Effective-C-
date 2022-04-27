#include "SharedPtr.h"
#include "AutoPtr.h"
#include <iostream>

class Base
{
private:
    int m_base;
public:
    virtual void Fun()
    {
        std::cout<<"Base::Fun()"<<std::endl;
    }
};

class Drived:public Base
{
private:
    int m_drived;
public:
    virtual void Fun()
    {
        std::cout<<"Drived::Fun()"<<std::endl;
    }

};


int main()
{
    SharedPtr<int> a= new int(100);//调用SharedPtr(T* ptr)构造函数
    std::cout<<"Construct MySharedPtr<int>"<<std::endl;
    std::cout<<"UseCount is: "<<*(a.getCount())<<std::endl;
    SharedPtr<int> b = a;
    std::cout<<"Cur UseCount is: "<<*(b.getCount())<<std::endl;

    SharedPtr<Base> TestNull;
    if(TestNull.IsNull())
    {
        std::cout<<"Null"<<std::endl;
    }
    SharedPtr<Base> Base_ptr = new Drived();  //基类智能指针指向派生类对象
    SharedPtr<Drived> Drived_ptr = new Drived();//派生类智能指针指向派生类对象
    Base_ptr->Fun();

    SharedPtr<Base> Base_ptr2 = SharedPtr<Base>::UpCast(Drived_ptr);//向上转型：实现派生类智能指针转换成基类智能指针
    if(!Base_ptr2.IsNull())
    {
        std::cout<<"UpCast"<<std::endl;
    }

    { 
        SharedPtr<Base> Base_ptrTest = new Drived();
        Drived_ptr = SharedPtr<Drived>::DownCast(Base_ptrTest);//向下转型：指向派生类对象的基类智能指针转换回派生类智能指针
        if(!Drived_ptr.IsNull())
        {
            std::cout<<"DownCast"<<std::endl;
        }
        //应该是2，Base_ptrTest和Drived_ptr
        std::cout<<"Drived_ptr's UseCount is: "<<*(Drived_ptr.getCount())<<std::endl;

        //应该是1，Base_ptr2
        std::cout<<"Base_ptr2's UseCount is: "<<*(Base_ptr.getCount())<<std::endl;
    }



    system("pause");
    return 0;




}