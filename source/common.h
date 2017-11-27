//
//  common.h
//  calcD_from_fas
//  Copyright (c) 2013 Bruno Nevado. GNU license.
//

#ifndef __calcD_fom_fas__common__
#define __calcD_fom_fas__common__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

std::vector <std::string>  msplit( const std::string& s , std::string delim );
bool contains_chars_not_in_string( std::string test, std::string allowed);
bool is_abba(std::string in);
bool is_baba(std::string in);

#endif
