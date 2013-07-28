//
//  CPPUtils.h
//  CPPUtils
//
//  Created by wangxu on 13-7-23.
//  Copyright (c) 2013å¹?wangxu. All rights reserved.
//

#ifndef CPPUtils_CPPUtils_h
#define CPPUtils_CPPUtils_h

#include <string>
#include <vector>

class CPPUtils
{
public:
    static void split(const std::string& str,char sep,std::vector<std::string>& out);
	static void combine(const std::vector<std::string>& arr,char sep,std::string& out);
	static void combine(const std::vector<std::string>& arr,char sep,char buffer[]);
};

#endif
