//
//  fasta.cpp
//  Created by Bruno Nevado on 10/02/2014.
// Copyright (c) 2014 Bruno Nevado. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <locale>

#include "fasta.h"

fasta::fasta ( int num_inds, int len ){
    if(len == 0){
        matrix.reserve(num_inds);
    }
    else{
        matrix.resize(num_inds);
        
        for(int i = 0; i < num_inds; i++)
            matrix.at(i).resize(len);
    }
}

void fasta::read_fasta_file( std::string infas){
    int cind = 0;
    matrix.clear();
    std::locale loc;
    std::string line;
    std::ifstream infile_fas (infas.c_str());
    if (infile_fas.is_open())
    {
        infile = infas;
        while ( ! infile_fas.eof() )
        {
            getline(infile_fas, line);
            if( line == ""){
                continue;
            }
            if( line[0] == '>' ){
                cind++;
                matrix.resize(cind);
                std::string name = line.substr(1);
                names.push_back(name);
            }
            else {
                matrix.at(cind-1).append( line );
            }
        }
    }
    else {
        std::cerr << "ERROR (read_fasta_file): Unable to open infile " << infas << "\n" ;
        exit(1);
    }
    infile_fas.close();
    for (unsigned int l = 0; l < matrix.size(); l++) {
        for (unsigned s = 0; s < matrix.at(l).length(); s++) {
            matrix.at(l).at(s) = tolower(matrix.at(l).at(s), loc);
        }
    }
}

std::vector <int > fasta::count_abba_baba ( std::string H1, std::string H2, std::string H3, std::string Out, bool all, bool strict){
    std::vector <int> counts (2,0);
    std::vector <int> indexes (4, -1); // will contain zero-based indexes of H1, H2,H3 and Out
    for( unsigned int i = 0; i < names.size(); i++ ){
        if( names.at(i) == H1 ){
            indexes.at(0) = i;
        }
        else if ( names.at(i) == H2 ){
            indexes.at(1) = i;
        }
        else if ( names.at(i) == H3 ){
            indexes.at(2) = i;
        }
        else if ( names.at(i) == Out ){
            indexes.at(3) = i;
        }
    }
    for(unsigned int i = 0; i < indexes.size(); i++){
        if(indexes.at(i) == -1){
            std::cerr << "WARNING: some species missing in file " << this->infile << "(looking for " << H1 << "," << H2 << "," << H3 << "," << Out << ")" << std::endl;
            if(!strict){
                // returns all 0s for this file
                return counts;
            }
        }
    }
    for (unsigned int isite = 0; isite < num_bases(); isite++) {
        std::string column;
        column.push_back( matrix.at(indexes.at(0)).at(isite));
        column.push_back( matrix.at(indexes.at(1)).at(isite));
        column.push_back( matrix.at(indexes.at(2)).at(isite));
        column.push_back( matrix.at(indexes.at(3)).at(isite));
        if( contains_chars_not_in_string( column, "acgt" ) ){
            continue;
        }
        if( is_abba(column)  ){
            counts.at(0)++;
            if(!all)
                return counts;
        }
        else if (is_baba(column)){
            counts.at(1)++;
            if(!all){
                return counts;
            }
        }
    }
    return counts;
}

