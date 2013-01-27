#ifndef _LINKFUNC_H_
#define _LINKFUNC_H_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

class LinkFunc {
  
 public:
  
  virtual gsl_vector *init_mv(gsl_vector *Y, int n)=0;
  virtual gsl_vector *compute_Z(gsl_vector *Y, gsl_vector *mv, int n)=0;
  virtual gsl_vector *compute_weights(gsl_vector *mv, int n)=0;
    
  virtual gsl_vector *compute_mv(gsl_vector *bv, gsl_matrix *Xv,int n, int p)=0;
  

};
  
#endif
