#include "header.h"


int main(int argc, char *argv[]){
    int state;
    DBcreator();//confirma se o ficheiro existe e se não existir cria-o
  if (argc!=1) // ver se foi iniciado com algum argumento
	{
    if(strcmp(argv[1],"adduser")==0)
		  addUser(argv[2]);

	}
	else{

  }
	

	return 0;
}


/*

/**/
/*


v

/*
void printclient(char mensagem[]){

}*/

//void newsms(void *socket_desc){
//	int sock = *(int*)socket_desc;
//}