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
			char cmd;
			//incialização
			//char *testvar = strdup(argv[0]);
			//printf("%s %c\n",argv[1],argv[1][i] );
			for (i = 0; argv[1][i] != '@'; ++i)//separa o user
			{
				user[i]=argv[1][i];
				//printf("%c",user[i] );
			}
			user[i]='\0';
			//ip handle
			
			for (++i; argv[1][i] != ':'; ++i)//separa o user
			{
				ipserver[n]=argv[1][i];
				++n;
			}
			ipserver[n]='\0';
			//printf("%s %d\n",ipserver,port ); //debug
			//porta

			for (++i; argv[1][i] != '\0'; ++i)
			{
				if (isdigit(argv[1][i])==0)
				{
					//printf("%c %d \n",argv[1][i],i );
					perror("Porta inválida.");
					exit(1);
				}
				port=port*10+(argv[1][i]-'0');

			}
			//printf("%s %d\n",ipserver,port );
			//incio da socket//atençao ao tmanho do user
			sock=socket(AF_INET, SOCK_STREAM, 0);
			if (sock==-1)
			{
				perror("A criação da socket falhou.");
				exit(1);
			}
			//atribuir caracteristicas ao servidor 
			server.sin_addr.s_addr = inet_addr( ipserver );//
			server.sin_family = AF_INET;
			server.sin_port = htons( port );//port

			//ligação ao servidor
			if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
			{
				perror("A conexão falhou.");
				exit(1);
			}else
			puts(" A conexão ao servidor foi efetuada com sucesso.");
			
			i=-1; //incicar estado
			recv(sock, message,sizeof(message), 0); //recebe a mensagem de controlo do servidor, dando origem a correspondecia de user
			do{
				if (i!=-1){
					puts("Falhou o login. Tente de novo.");
					scanf("%s",user);
				}
				bzero(message, sizeof(message));
				write(sock, user , strlen(user));
				recv(sock, message,sizeof(message), 0);//ver o maximo d eparammetros existemtes
				i=commandrcv(message[0]); //vai traduzir a string para int
			}while(i!=1 && i!=2);

			if (i==1)
			{
				perror("Atingiu o limite de logins.");
				exit(1);
			}
			/*Metodo para as passwoard's */
			i=-1;
			do{
				bzero(message, sizeof(message));
				if (i!=-1)
					puts("Falhou a passwoard. Tente de novo.");

				printf("Passwoard: "); //falta desligar o echo da password
				
				scanf("%s", message);
				write(sock, message , strlen(message));
				bzero(message, sizeof(message));
				recv(sock, message,sizeof(message), 0);//ver o maximo d eparammetros existemtes
				//printf("%s codigo \n",message);
				i=commandrcv(message[0]);
			}while(i!=1 && i!=2);

			fflush(stdout);
			if (i==1)
			{
				perror("Atingiu o limite de tentativas para a passwoard.");
				exit(1);
			}
			//puts("entrada");
			//criação da thread
			pthread_t sniffer_thread;
			newsock = malloc(4);
          	*newsock = sock;
                   
          if( pthread_create( &sniffer_thread , NULL ,  SMSreceaver ,(void*) sock) < 0)
          {
              perror("could not create thread");
              exit(1);
          }
          	mainprinter();
			while(1){
				//printf("*****MENU******\n1)Listar os utilizadores online.\n2)Enviar nova mensagem.\n3)Log out.\n");
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
					default:
						puts("Opção inválida.");
				}
			}
		}
		else{
			perror("Chamada do cliente sem argumentos!");
			exit(1);
		}
		return 0;
	} 
