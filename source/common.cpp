//
//  common.cpp
//  calcD_from_fas
//  Copyright (c) 2013 Bruno Nevado. GNU license.
//

#include <fstream>
#include <algorithm>
#include "common.h"

std::vector <std::string>  msplit( const std::string& s , std::string delim ){
    std::vector <std::string> res;
    unsigned long start = 0U;
    unsigned long end = s.find(delim);
    while (end != std::string::npos)
    {
        res.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    res.push_back(s.substr(start, end - start));
    return res;
}


bool contains_chars_not_in_string( std::string test, std::string allowed){
    std::size_t found;
    for (unsigned int i = 0; i < test.size(); i++) {
        found=allowed.find(test.at(i));
        if (found==std::string::npos)
            return true;
    }
    return false;
}

bool is_abba(std::string in ){
    if( in.at(0) == in.at(3) && in.at(1) == in.at(2) && in.at(0) != in.at(1) ){
        return true;
    }
    else{
        return false;
    }
}

bool is_baba(std::string in){
    if( in.at(0) == in.at(2) && in.at(1) == in.at(3) && in.at(0) != in.at(1) ){
        return true;
    }
    else{
        return false;
    }
}

