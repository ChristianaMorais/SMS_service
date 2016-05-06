#include "header.h"
#include "ctype.h"
#include <stdlib.h>
#include <pthread.h>
#include "time.h"
char user[30];
char message[1024];
int main(int argc, char const *argv[])
{
	if (argc==2)
	{
		//bzero(message, sizeof(message));
		
		int sock;
		struct sockaddr_in server;
		char ipserver[10];//pode falhar por causa da terminar da strting
		int port=0, i=0,n=0,*newsock;
		char c;

		//utilizador
		for (i = 0; argv[1][i] != '@'; ++i){//separa o user
			user[i]=argv[1][i];
		}
		user[i]='\0';

		//ip
		for (++i; argv[1][i] != ':'; ++i){//separa o ip
			ipserver[n]=argv[1][i];
			++n;
		}
		ipserver[n]='\0';
		
		//porta
		for (++i; argv[1][i] != '\0'; ++i){
			if (isdigit(argv[1][i])==0){
				perror("Porta inválida.");
				exit(EXIT_FAILURE);
			}
			port=port*10+(argv[1][i]-'0');
		}

		//Criação da socket
		sock=socket(AF_INET, SOCK_STREAM, 0);
		if (sock==-1){
			perror("A criação da socket falhou.");
			exit(EXIT_FAILURE);
		}

		//atribuir caracteristicas ao servidor 
		server.sin_addr.s_addr = inet_addr( ipserver );//
		server.sin_family = AF_INET;
		server.sin_port = htons( port );//port

		//ligação ao servidor
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
			perror("A conexão falhou.");
			exit(EXIT_FAILURE);
		}else
		puts(" A conexão ao servidor foi efetuada com sucesso.");
		
		i=-1; //incicar estado serve para nao mostar indicaçoes de login na primeira tentativa e serve de tbm para receber as mesangens do servidor
		
		do{
			if (i!=-1){
				puts("Falhou o login. Tente de novo.");
				printf("Login: ");
				scanf("%s",user);
			}
			bzero(message, sizeof(message));
			write(sock, user , strlen(user));
			recv(sock, message,sizeof(message), 0);//ver o maximo d eparammetros existemtes
			i=commandrcv(message[0]); //vai traduzir a string para int
		}while(i!=1 && i!=2);


		if (i==1){
			perror("Atingiu o limite de logins.");
			exit(EXIT_FAILURE);
		}

		/*Metodo para as passwoard's */
		i=-1;
		__fpurge(stdin);

		do{
			bzero(message, sizeof(message));
			if (i!=-1)
				puts("Falhou a passwoard. Tente de novo.");

			printf("Passwoard: "); //falta desligar o echo da password
			i=0;

			c=getch();//input da passwoard
			while(c!='\n'){
				message[i]=c;
				++i;
				c=getch();
			}
			message[i]='\0';
			
			printf("\n");
			write(sock, message , strlen(message));
			bzero(message, sizeof(message));
			recv(sock, message,sizeof(message), 0);//ver o maximo d eparammetros existemtes
			i=commandrcv(message[0]);
		}while(i!=1 && i!=2);

		fflush(stdout);
		if (i==1){
			perror("\nAtingiu o limite de tentativas para a passwoard.");
			exit(EXIT_FAILURE);
		}
		
		pthread_t sniffer_thread;
		newsock = malloc(4);
      	*newsock = sock;
               
      	if( pthread_create( &sniffer_thread , NULL ,  SMSreceaver ,(void*) sock) < 0){
          perror("Não conseguiu criar a thread");
          exit(EXIT_FAILURE);
      	}

      	printf("\nUtilizador %s autenticado. Pode começar a usar o sistema!\n",user);
      	mainprinter();
		while(1){
			scanf("%d",&i);
			bzero(message, sizeof(message));
			switch(i){
				case 1: //listar online
					write(sock,"1",1);
					break;
				case 2://enviar sms
					write(sock,"2",1);
					SMScreater(sock);
					break;
				case 3: //terminar programa
					write(sock,"9",1);
					break;
				case 4:	//alterar a passwoard
					write(sock,"4",1);
					__fpurge(stdin);
					passwoardChangerSend(sock);
					break;
				default:
					puts("Opção inválida.");
			}
		}
	}
	else{
		perror("Chamada do cliente sem argumentos!");
		exit(EXIT_FAILURE);
	}
	return 0;
} 
