//
//  CPPUtils.cpp
//  CPPUtils
//
//  Created by wangxu on 13-7-23.
//  Copyright (c) 2013年 wangxu. All rights reserved.
//

#include "CPPUtils.h"

using namespace std;

void CPPUtils::split(const string &str, char sep, vector<string> &out)
{
    string substring;
    string::size_type start = 0;
    string::size_type index = 0;
    
    do
    {
        index = str.find_first_of(sep,start);
        if (index != string::npos)
        {
            substring = str.substr(start,index-start);
            out.push_back(substring);
            start = str.find_first_not_of(sep,index);
            if (start == string::npos)
            {
                return;
            }	
        }
    }while(index != string::npos);
    
    //the last token
    substring = str.substr(start);
    out.push_back(substring);
}