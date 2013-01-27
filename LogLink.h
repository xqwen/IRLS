#ifndef _LOGLINK_H_
#define _LOGLINK_H_

#include "LinkFunc.h"



class LogLink : public LinkFunc{
  gsl_vector *init_mv(gsl_vector *Y, int n);
  gsl_vector *compute_Z(gsl_vector *Y, gsl_vector *mv, int n);
  gsl_vector *compute_weights(gsl_vector *mv, int n);
    
  gsl_vector *compute_mv(gsl_vector *bv, gsl_matrix *Xv,int n, int p);
  
};

#endif
