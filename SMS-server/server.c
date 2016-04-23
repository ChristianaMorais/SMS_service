#include "header.h"

int main(int argc, char *argv[]){
	int state;
   if (argc!=1) // ver se foi iniciado com algum argumento
  {
  	if(strcmp(argv[1],"adduser")==0){
  		addUser(argv[2]);
      printf("Utilizador %s foi adicionado.\n",argv[2]);
    }
  }
  else{
   //char command[50];
   //strcpy( command, "clear" );
   system("clear");

    DBreader();
    startServer();


  }

  return 0;
}


