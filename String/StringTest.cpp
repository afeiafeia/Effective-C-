#include "String.h"
#include "exceptdef.h"
#include <iostream>


int main()
{
    String s1("abcdefg");
    String s2 = s1;
    printf("s1起始地址=%d\n",s1.begin());
    printf("s2起始地址=%d\n",s2.begin());

    std::cout<<s1[0]<<"     "<<s2[0]<<std::endl;
    std::cout<<"读操作后"<<std::endl;
    printf("s1起始地址=%d\n",s1.begin());
    printf("s2起始地址=%d\n",s2.begin());


    s1[2]='m';
    std::cout<<"写操作后"<<std::endl;
    printf("s1起始地址=%d\n",s1.begin());
    printf("s2起始地址=%d\n",s2.begin());

    return 0;



}
