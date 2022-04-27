#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H


namespace mystl
{

    template<bool value,class T>
    struct enable_if{};


    template<class T>
    struct enable_if<true,T>
    {
        typedef T   value_type;
    };


}
#endif
