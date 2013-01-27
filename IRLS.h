#ifndef _IRLS_H_
#define _IRLS_H_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include "LinkFunc.h"

class IRLS {
  
 private:
  gsl_vector *Y;  // response vector
  gsl_matrix *X; // design matrix

  gsl_vector *fit_coef;
  
  int n; // sample size;
  int p; // number of parameters (including intercept)

  LinkFunc *link;
  
  

  
 public:

  IRLS(const char * link_type);
  ~IRLS();
  void load_data(vector<double> & yv, vector<vector<double> > &Xv);
  void fit_model();
  vector<double> get_fit_coef();
};

#endif
