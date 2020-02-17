//
//  STL_Set.cpp
//  test
//
//  Created by archer on 2020/2/17.
//  Copyright © 2020 apple. All rights reserved.
//

#include "STL_Set.hpp"
#include <set>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;

int main()
{
    set<int> seta{4,2,7,1,4,2};
    set<int> setb{10,2,7,15};
    set<int> result;
    
    
    //求并集
    set_union(begin(seta), end(seta), begin(setb), end(setb), inserter(result,begin(result)));
    
    cout<<"seta setb 并集："<<endl;
    for(auto i:result)
    {
        cout<<i<<" ";
    }
    cout<<endl;

    cout<<"seta setb 交集"<<endl;
    result.clear();
    //求交集
    set_intersection(begin(seta), end(seta), begin(setb), end(setb), ostream_iterator<int> {cout," - "});
    

    cout<<endl;
    
    
    return 0;
}
