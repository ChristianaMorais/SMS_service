#include "header.h"

int main(int argc, char *argv[]){
	int i,b;
   if (argc!=1){// ver se foi iniciado com algum argumento
   	DBcreator();
    if(strcmp(argv[1],"adduser")==0){
	if (UserNumber()!=0)
       DBreader();
  		addUser(argv[2]);      
    }
    if(strcmp(argv[1],":")==0){//alteração de porta
    	i=0;
    	b=0;
    	while(argv[2][i]!='\0'){
    	  if (isdigit(argv[2][i])==0){
    	    puts("Porta inválida!");
    	    exit(EXIT_FAILURE);
    	  }
    	  b=b*10+(argv[2][i]-'0');//converte string para int
    	  ++i;
    	}
    	if (b < 1 || b > 65536){ // As porta so podem ser de 1 a 65536
    	  puts("Porta inválida!");
    	  exit(EXIT_FAILURE);
    	}
    	portChanger(b);//muda a porta no ficheiro de configurações
    	printf("A porta do servidor foi alterada para a porta %d.\n",b);		      
    }
  }
  else{
    system("clear"); //limpa a linha de comandos
    onlineLogreset();//faz reset ao ficheiro auxiliar de utilizadores online
    if (UserNumber()!=0)
        DBreader();//lê a base de dados
    startServer(); //inicia o servidor


  }

  return 0;
}


