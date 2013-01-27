using namespace std;

#include "controller.h"
#include <stdlib.h>

int main(int argc, char **argv){
  

  char data_file[128];
  

  memset(data_file,0,128); 


    
  for(int i=1;i<argc;i++){
     
    
    if(strcmp(argv[i], "-d")==0 || strcmp(argv[i], "-data")==0){
      strcpy(data_file,argv[++i]);
      continue;
    }
    

   fprintf(stderr, "Error: unknown option \"%s\"\n",argv[i]);
   exit(1);
   
  }

  controller con(data_file);
  con.fit_poisson();
}


