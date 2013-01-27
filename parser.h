#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string.h>

class parser {


 public:
  
  vector<vector<double> > geno_vec;
  vector<vector<double> > pheno_vec;
  
  map<int, string> pheno_map;
  map<int, string> geno_map;
  

  void process_data(char *filename);
  void output();
  
 private:
  
  void process_line(string line);

};
