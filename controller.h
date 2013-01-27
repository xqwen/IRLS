#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "parser.h"


class controller {
  
 private:

  parser pars;

 public:
  
  controller(char *data_file);
  
  void fit_poisson();
  
};  


#endif
