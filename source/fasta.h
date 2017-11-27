//
//  fasta.h
//  Created by Bruno Nevado on 10/02/2014.
// Copyright (c) 2014 Bruno Nevado. All rights reserved.
//

#ifndef __calcD_fom_fas__fasta__
#define __calcD_fom_fas__fasta__


#include <iostream>
#include <vector>
#include <string>

#include "common.h"

class fasta {
    std::vector < std::string > matrix;
    std::vector <std::string> names;
    std::string infile;
public:
    fasta( int num_inds, int len = 0 );
    unsigned int num_lines () const {return int ( matrix.size() );}
    unsigned int num_bases () const {return int ( matrix[0].size() );}
    void read_fasta_file ( std::string in ) ;
    void info_to_stdout(){ std::cout << "Read " << num_lines() << " sequences from file " << infile << std::endl;  };
    
    std::string get_infile(){ return infile; };
    std::vector <int > count_abba_baba ( std::string H1, std::string H2, std::string H3, std::string Out, bool all, bool strict );
    std::string name_at ( int ind0  ) { return names.at(ind0); };
};


#endif /* defined(__calcD_fom_fas__fasta__) */
