using namespace std;

#include "controller.h"
#include "IRLS.h"
#include <fstream>
#include <sstream>




controller::controller(char *data_file){
  pars.process_data(data_file);
}


void controller::fit_poisson(){
  IRLS irls("log-link");  
  irls.load_data(pars.pheno_vec[0], pars.geno_vec);
  irls.fit_model();
  vector<double> coev = irls.get_fit_coef();
  vector<double> sev = irls.get_stderr();
  
  printf("coef: "); 
  for(int i=0;i<coev.size();i++){
    printf("%7.5f  ",coev[i]);
  }
  printf("\n");
  
  printf("  se: "); 
  for(int i=0;i<sev.size();i++){
    printf("%7.5f  ",sev[i]);
  }
  printf("\n");
  
  
}
