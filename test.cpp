/** \filetest.cpp
*
* This file uses the IRLS class on data files to check the results
* with R's glm(). See the script test.R for more details.
* Copyright (C) 2013,2017 Xioaquan Wen, Timothee Flutre
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <cmath>
#include <cstring>

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_cdf.h>

#include "IRLS.h"

int main(int argc, char ** argv)
{
  // parse the command-line
  string file_y = "test_y.dat",
    file_X = "test_X.dat",
    file_offset;
  bool quasi_lik = false;
  for(int i = 1; i < argc; ++i){
    if(strcmp(argv[i], "-y")==0){
      file_y = string(argv[++i]);
      continue;
    }
    if(strcmp(argv[i], "-X")==0){
      file_X = string(argv[++i]);
      continue;
    }
    if(strcmp(argv[i], "--off")==0){
      file_offset = string(argv[++i]);
      continue;
    }
    if(strcmp(argv[i], "-q")==0){
      quasi_lik = true;
      continue;
    }
  }
  
  cout << "load response vector " << file_y << endl;
  size_t N = 200;
  gsl_vector * y = gsl_vector_alloc(N);
  FILE * f = fopen(file_y.c_str(), "r");
  gsl_vector_fread(f, y);
  fclose(f);
  
  cout << "load predictor matrix " << file_X << endl;
  size_t P = 5+1;
  gsl_matrix * X = gsl_matrix_alloc(N, P);
  f = fopen(file_X.c_str(), "r");
  gsl_matrix_fread(f, X);
  fclose(f);
  
  cout << "load offset vector " << file_offset << endl;
  gsl_vector * offset = NULL;
  if(! file_offset.empty()){
    offset = gsl_vector_alloc(N);
    f = fopen(file_offset.c_str(), "r");
    gsl_vector_fread(f, offset);
    fclose(f);
  }
  
  cout << "fit the model" << endl;
  IRLS irls("log-link");
  irls.link->quasi = quasi_lik;
  irls.set_data(y, X, offset);
  irls.fit_model();
  vector<double> coev = irls.get_coef();
  vector<double> sev = irls.get_stderr();
  
  // print the results
  printf("dispersion=%.4f\n", irls.get_dispersion());
  printf("%10s%12s%12s%15s\n", "", "Estimate", "Std.Error", "p-value");
  for(size_t i = 0; i < coev.size(); ++i){
    printf("X%-9zu%12.9f%12.8f", i, coev[i], sev[i]);
    if(! irls.link->quasi)
      printf("%15.6e\n", 2 * gsl_cdf_gaussian_P(-fabs(coev[i]/sev[i]), 1.0));
    else
      printf("%15.6e\n", 2 * gsl_cdf_tdist_P(-fabs(coev[i]/sev[i]), N-irls.get_rank_X()));
  }
  
  gsl_vector_free(y);
  gsl_matrix_free(X);
  gsl_vector_free(offset);
  
  return EXIT_SUCCESS;
}
