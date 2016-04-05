#include "header.h"

typedef struct { //definição do novo tipo utilizador
	char login[30];
	char password[30];
} utilizador;

static utilizador Dados[UserNumber()]; //alem de crirar  matriz correspondente tambem cria o ficheiro se não existir 

int main(int argc, char *argv[]){
	int state;
   if (argc!=1) // ver se foi iniciado com algum argumento
  {
  	if(strcmp(argv[1],"adduser")==0)
  		addUser(argv[2]);
  }
  else{


  }

  return 0;
}


