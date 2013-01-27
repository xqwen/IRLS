using namespace std;

#include "LogLink.h"
#include <gsl/gsl_blas.h>
#include <math.h>

gsl_vector * LogLink::init_mv(gsl_vector *Y, int n){
  
  gsl_vector *mv = gsl_vector_calloc(n);
  
  for(int i=0;i<n;i++){
    double val = gsl_vector_get(Y,i);
    double mval = val;
    if(val == 0){
      mval = 0.01;
    }
    
    gsl_vector_set(mv,i,mval);
  }
  
  return mv;
}



gsl_vector *LogLink::compute_Z(gsl_vector *Y, gsl_vector *mv, int n){
  
  gsl_vector *Z = gsl_vector_calloc(n);
  for(int i=0;i<n;i++){
    double mu = gsl_vector_get(mv,i);
    double y = gsl_vector_get(Y,i);
    double val = log(mu)+(1.0/mu)*(y-mu);
    gsl_vector_set(Z,i,val);
  }

  return Z;
}

gsl_vector *LogLink::compute_weights(gsl_vector *mv, int n){
  
  gsl_vector *w = gsl_vector_calloc(n);
  for(int i=0;i<n;i++){
    double val = gsl_vector_get(mv,i);
    gsl_vector_set(w,i,val);
  }
  return w;

}



gsl_vector *LogLink::compute_mv(gsl_vector *bv, gsl_matrix *Xv,int n, int p){
  
  gsl_matrix *beta = gsl_matrix_calloc(p,1);
  
  for(int i=0;i<p;i++){
    gsl_matrix_set(beta,i,0,gsl_vector_get(bv,i));
  }

  gsl_matrix *fit = gsl_matrix_calloc(n,1);
  gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1,Xv,beta,0,fit);
  
  gsl_vector *mv = gsl_vector_calloc(n);

  for(int i=0;i<n;i++){
    double val = gsl_matrix_get(fit,i,0);
    gsl_vector_set(mv,i,exp(val));
  }
  
  gsl_matrix_free(fit);
  gsl_matrix_free(beta);

  return mv;
}
