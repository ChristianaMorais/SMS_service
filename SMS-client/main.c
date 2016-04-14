#include "header.h"
#include "ctype.h"
#include <stdlib.h>
char user[30];
char message[1024];
int main(int argc, char const *argv[])
{
	if (argc==2)
	{
		
		int sock;
		struct sockaddr_in server;
		
			char ipserver[10];//pode falhar por causa da terminar da strting
			int port=0, i=0,n=0;
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
				perror("O conexão falhou.");
				exit(1);
			}else
			puts("conexão efetuada com sucesso. A fazer login");
			
			i=-1; //incicar estado
			//inicar login
			//while(1){
			//puts(user);
			recv(sock, message,30, 0);
			printf("%s codigo recebido\n",message );
			do{
				if (i!=-1){
					puts("Falhou o login. Tente de novo.");
					scanf("%s",user);
				}
				//puts(user);
				write(sock, user , strlen(user));
				//puts(user);
				//printf("%s login enviado \n",user );
				recv(sock, message,30, 0);//ver o maximo d eparammetros existemtes
				//printf("%s\n",message );

				//cmd=message[0];
				i=commandrcv(message[0]);
			}while(i!=1 && i!=2);

			if (i==1)
			{
				perror("Atingiu o limite de logins.");
				exit(1);
			}
			//iniciar passwoard
			i=-1;
			do{
				if (i!=-1)
				puts("Falhou a passwoard. Tente de novo.");
				printf("Passwoard: ");
				scanf("%s", message);
				write(sock, message , strlen(message));
				puts("espera da ligação");
				recv(sock, message,30, 0);//ver o maximo d eparammetros existemtes
				printf("%s codigo \n",message);
				i=commandrcv(message[0]);
			}while(i!=1 && i!=2);

			if (i==1)
			{
				perror("Atingiu o limite de tentativas para passwoard.");
				exit(1);
			}
			//puts("entrada");
			while(1){
				printf("*****MENU******\n1)Listar os utilizadores online.\n2)Enviar nova mensagem.\n3)Log out.\n");
				scanf("%d",&i);
				switch(i){
					case 1: //listar online
						write(sock,"1",1);
						recv(sock,message,1024,1);
						puts(message);
						break;
					case 2://enviar sms
						write(sock,"2",1);
						i=SMScreater(sock);
						if (i==0)
							puts("Mensagem enviada com sucesso.");
						else
							puts("Mensagem não enviada.");
						break;
					case 3: //terminar programa
						write(sock,"9",1);
						puts("Log out efetuado.");
						close(sock);
						return 0;
						break;
					default:
						puts("Opção inválida.");
				}

				SMSreceaver(sock);
			}

			//}
			close(sock);
	/*	}
		else
		{
			perror("Sintaxe não reconhecida.");
			exit(1);
		}*/

		}
		else{
			perror("Chamada do cliente sem argumentos!");
			exit(1);
		}
		return 0;
	} 
