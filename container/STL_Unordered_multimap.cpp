//
//  STL_Unordered_multimap.cpp
//  test
//
//  Created by archer on 2020/2/16.
//  Copyright © 2020 apple. All rights reserved.
//

#include "STL_Unordered_multimap.hpp"
#include <unordered_map>
#include <iostream>
#include <string>
using namespace std;

int main()
{
    //允许重复key
    unordered_multimap<string, int> a;
    a.insert({"Tom",100});
    a.insert({"Tom",200});
    a.insert({"Yun",300});
    a.insert({"Sun",200});
    a.insert({"Meck",200});
    cout<<"map size: "<<a.size()<<endl;
    for(auto i:a)
    {
        cout<<i.first<<" "<<i.second<<endl;
    }
    //由于允许重复的可以，因此使用find  eauql_range 来访问元素
    //find 返回第一个匹配的key的元素迭代器
    auto ptr=a.find("Yun");
    if(ptr!=end(a))
    {
        cout<<"use find Key-Yun Value="<<ptr->second<<endl;
    }
    //equal_range 返回一个pair迭代器范围 给出所有匹配key的迭代器
    auto k=a.equal_range("Tom");
    if(k.first!=end(a))
    {
        cout<<"use equal_range find Tom: "<<endl;
        for(auto i=k.first;i!=k.second;++i)
        {
            cout<<i->first<<" "<<i->second<<endl;
        }
    }
    //使用erase删除 元素 方法类似 map
    return  0;
}
