/*
calcD_from_fas
Calculates Patterson's D-statistic from a set of aligned fasta files. Can use only first abbababa site from each file (test assumes loci are unlinked).
See: Durand, Eric Y., et al. Testing for ancient admixture between closely related populations. Mol Biol Evol 2011.
 
Copyright (c) 2014 Bruno Nevado. GNU license.
Created by bnevado on 01/07/2014.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#include "common.h"
#include "fasta.h"
#include "args.h"

void help(){
    std::cout << "###################\n  calcD_from_fas v1 27112017 \n###################" << std::endl;;
    std::cout << "Makes abba-baba tests using fasta files" << std::endl;;
    std::cout << "Usage: calcD_from_fas -folder /home/data -contigs all_contigs.txt -tests tests.txt -outfile result.txt -allsites 1/0" << std::endl;
    std::cout << "\t-folder: folder containing fasta files." << std::endl;
    std::cout << "\t-contigs: list of contigs within 'folder' to use in test." << std::endl;
    std::cout << "\t-tests: tests to perform, each line should contain sequence names to use (comma separated, order: H1,H2,H3,Out; lines starting with # are ignored)." << std::endl;
    std::cout << "\t-outfile: file to write results to." << std::endl;
    std::cout << "\t-allsites: if 1, will use all sites from all files. if 0, will use only first abba/baba site from each fasta file." << std::endl;
    std::cout << "\t-strict: if 1, will stop with an error if a species is missing in any fasta file. If 0, will skip that file." << std::endl;
}

int main(int argc, const char * argv[])
{
    
    sargs myargs;
    try{
        myargs = args::getargs(argc, argv, std::vector<std::string> {"folder","contigs","tests","outfile"}, std::vector<std::string> {"allsites","strict"}, std::vector<std::string>  {}, std::string {}, std::string {}); }
    catch (std::string e){
        std::cout << " Args failed: " << e << std::endl;
        help();
        exit(1);
    }
    
    std::string folder = myargs.args_string.at(0);
    std::string contigs = myargs.args_string.at(1);
    std::string tests = myargs.args_string.at(2);
    std::string outfile = myargs.args_string.at(3);
    bool allsites = myargs.args_booleans.at(0);
    bool strict = myargs.args_booleans.at(1);
    
    std::cout << "<calcD_from_fas> Running abba-baba test with " << ((allsites) ? "all sites" : "max 1 site per fasta file") << std::endl;
    std::vector< std::string > h1s;
    std::vector< std::string > h2s;
    std::vector< std::string > h3s;
    std::vector< std::string > outs;
    std::string cline;
    
    // GET TESTS TO PERFORM
    std::ifstream in_tests (tests.c_str());
    if(!in_tests.is_open()){
        std::cerr << "<calcD_from_fas> ERROR: unable to open for reading tests infile " << tests << std::endl;
        exit(1);
    }
    int nlines = 0;
    while( getline(in_tests, cline) ){
        nlines++;
        
        if( cline.at(0) == '#' || cline == "" ){
            continue;
        }
        std::vector<std::string> temp = msplit(cline, ",");
        try{
            h1s.push_back(temp.at(0));
            h2s.push_back(temp.at(1));
            h3s.push_back(temp.at(2));
            outs.push_back(temp.at(3));
        }
        catch(...){
            std::cerr << "<calcD_from_fas> ERROR: line " << nlines << " in test infile " << tests << " malformed (should be 'H1,H2,H3,Out')" << std::endl;
            exit(1);
        }
    }
    
    std::cout << "<calcD_from_fas> Read " << h1s.size() << " tests to perform from file " << tests << std::endl;
    
    // GET FASTA FILES
    std::vector < fasta > fasta_files;
    std::ifstream in_contigs (contigs.c_str());
    if(!in_contigs.is_open()){
        std::cerr << "<calcD_from_fas> ERROR: unable to open for reading contigs infile " << contigs << std::endl;
        exit(1);
    }
    while( getline( in_contigs, cline) ){
        std::stringstream ss;
        ss << folder;
        ss << "/";
        ss << cline;
        fasta afasta(10);
        afasta.read_fasta_file(ss.str());
        fasta_files.push_back( afasta);
    }
    std::cout << "<calcD_from_fas> Read " << fasta_files.size() << " fasta files from folder " << folder << std::endl;
    
    // ABBA-BABA
    std::vector <int> abba ( h1s.size(), 0 );
    std::vector <int> baba ( h1s.size(), 0 );
    for (unsigned int ifasta = 0; ifasta < fasta_files.size(); ifasta++) {
        for (unsigned int itest = 0; itest < h1s.size(); itest++ ) {
            std::vector <int> res(2,0);
            try{
                res = fasta_files.at(ifasta).count_abba_baba(h1s.at(itest), h2s.at(itest), h3s.at(itest), outs.at(itest), allsites, strict);
            }
            catch(...){
                std::cerr << "ERROR processing file " <<fasta_files.at(ifasta).get_infile() << std::endl;
                exit(1);
            }
            abba.at(itest) += res.at(0);
            baba.at(itest) += res.at(1);
        }
        std::cout << "<calcD_from_fas> Finished processing file " << ifasta + 1 << " of " << fasta_files.size()  << std::endl;
    }
    
    std::ofstream fh_out(outfile);
    if( !fh_out.is_open()){
        std::cerr << "<calcD_from_fas> ERROR: Unable to open for writing file " << outfile << std::endl;
        exit(1);
    }
    fh_out << "H1\tH2\tH3\tOut\tnABBA\tnBABA\tD\tSD\tZ" << std::endl;
    for (unsigned int itest = 0; itest < h1s.size(); itest++ ) {
        fh_out << h1s.at(itest) << "\t" << h2s.at(itest) << "\t" << h3s.at(itest) << "\t" << outs.at(itest) << "\t" << abba.at(itest) << "\t" << baba.at(itest) << "\t" ;
        if ((abba.at(itest) + baba.at(itest)) == 0){
            fh_out << "NA\tNA\tNA" << std::endl;;
        }
        else{
            float d = float(abba.at(itest) - baba.at(itest)) / (abba.at(itest) + baba.at(itest));
            float sd = float(2 * sqrt( double( 0.25 / (abba.at(itest) + baba.at(itest)) ) ));
            float z = d / sd;
            fh_out << d  << "\t" <<  sd << "\t" << z << std::endl;
        }
    }
    fh_out.close();
    std::cout << "<calcD_from_fas> Finished, results written to " << outfile << std::endl;
    return 0;
}

