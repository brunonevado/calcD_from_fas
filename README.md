CALCD_FROM_FAS:  Calculates Patterson's D-statistic from a set of aligned fasta files.
  
Author: B. Nevado  
  
Usage:  
calcD_from_fas -folder /home/data -contigs all_contigs.txt -tests tests.txt -outfile result.txt -allsites 1/0  
    -folder: folder containing fasta files.  
    -contigs: list of contigs within 'folder' to use in test.  
    -tests: tests to perform, each line should contain sequence names to use (comma separated, order: H1,H2,H3,Out; lines starting with # are ignored).  
    -outfile: file to write results to.  
    -allsites: if 1, will use all sites from all files. if 0, will use only first abba/baba site from each fasta file.  
    -strict: if 1, will stop with an error if a species is missing in any fasta file. If 0, will skip that file.  
  
Output: text file (to -outfile) with number of abba and baba sites, D-statistic, and Z-score.  
    
Notes:  
    Positions with unknown bases are ignored.  
  
Installation (Linux):  
    git clone https://github.com/brunonevado/calcD_from_fas  
    cd calcD_from_fas  
    make  
    ./calcD_from_fas  

