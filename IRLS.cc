using namespace std;

#include "LogLink.h"
#include "string.h"
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <vector>
#include "IRLS.h"



IRLS::IRLS(const char * link_type){
  if(strcmp(link_type,"log-link")==0){
    link = new LogLink();
  }
  fit_coef = 0;
}

void IRLS::load_data(vector<double> & yv, vector<vector<double> > &Xv){
  
  n = yv.size();
  p = Xv.size()+1;
  
  Y = gsl_vector_calloc(n);
  X = gsl_matrix_calloc(n,p);
  
  for(int i=0;i<n;i++){
    gsl_vector_set(Y,i,yv[i]);
    for(int j=0;j<p;j++){
      double val = 1;
      if(j>=1)
	val = Xv[j-1][i];
      
      gsl_matrix_set(X,i,j,val);
    }
  }
  
  return;
}
 

void IRLS::fit_model(){
  
  gsl_vector *mv = link->init_mv(Y,n);
  
  double old_chisq = -1;
  while(1){
    
    gsl_vector *z = link->compute_Z(Y,mv,n);
    gsl_vector *w = link->compute_weights(mv,n);
  
    // weighted least square fitting
    gsl_vector *bv = gsl_vector_alloc(p);
    gsl_matrix *cov = gsl_matrix_alloc(p, p);
    gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc (n, p);
    double chisq;
    gsl_multifit_wlinear (X, w, z, bv, cov, &chisq, work);
    
    gsl_vector_free(z);
    gsl_vector_free (w);
    gsl_matrix_free (cov);
    gsl_multifit_linear_free (work);
    gsl_vector_free(mv);


    //convergence diagnosis here
    if(fabs(chisq - old_chisq)<1e-6){
 
      if(fit_coef !=0){
	gsl_vector_free(fit_coef);
      }
      fit_coef = bv;
      break;
    }
    
    old_chisq = chisq;
    mv = link->compute_mv(bv,X,n,p);
    gsl_vector_free(bv);
   
  }
  

}


vector<double> IRLS::get_fit_coef(){
  
  vector<double> coev;
  for(int i=0;i<p;i++){
    coev.push_back(gsl_vector_get(fit_coef,i));
  }
  return coev;
}
  


IRLS::~IRLS(){
  
  if(fit_coef !=0){
    gsl_vector_free(fit_coef);
  }

}
