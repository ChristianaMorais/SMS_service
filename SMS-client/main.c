#include "header.h"
#include "ctype.h"
#include <stdlib.h>
int main(int argc, char const *argv[])
{
	if (argc==2)
	{
		
		int sock;
		struct sockaddr_in server;
		char *message,user[30];
			char ipserver[10];//pode falhar por causa da terminar da strting
			int port=0, i;
			char cmd;
			//incialização
			char *testvar = strdup(argv[0]);

			for (i = 0; i <= 28 && testvar[i] != '@'; ++i)//separa o user
			{
				user[i]=testvar[i];
			}
			user[i]='\0';

			//ip handle

			for (++i; testvar[i] != ':'; ++i)//separa o user
			{
				ipserver[i]=testvar[i];
			}
			ipserver[i]='\0';

			//porta

			for (++i; testvar[i] != '\n'; ++i)
			{
				if (isdigit(testvar[i])==0)
				{
					perror("Porta inválida.");
					exit(1);
				}
				port=port*10+(testvar[i]-'0');

			}


			//incio da socket//atençao ao tmanho do user
			sock=socket(AF_INET, SOCK_STREAM, 0);
			if (sock==-1)
			{
				perror("A criação da socket falhou.");
				exit(1);
			}
			//atribuir caracteristicas ao servidor 
			server.sin_addr.s_addr = inet_addr(ipserver);
			server.sin_family = AF_INET;
			server.sin_port = htons( port );

			//ligação ao servidor
			if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
			{
				perror("O conexão falhou.");
				exit(1);
			}
			i=-1; //incicar estado
			//inicar login
			do{
				if (i!=-1){
					puts("Falhou o login. Tente de novo.");
					scanf("%s\n",user);
				}
				write(sock, user , strlen(user),0);
				recv(sock, message,1, 0);//ver o maximo d eparammetros existemtes
				cmd=message[0];
				i=commandrcv(cmd);
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
				scanf("%s \n", message);
				write(sock, message , strlen(message),0);
				recv(sock, message,1, 0);//ver o maximo d eparammetros existemtes
				i=commandrcv(cmd);
			}while(i!=1 && i!=2);

			if (i==1)
			{
				perror("Atingiu o limite de tentativas para passwoard.");
				exit(1);
			}

			while(i!=-1){
				printf("*****MENU******\n1)Listar os utilizadores online.\n2)Enviar nova mensagem.\n3)Log out.\n");

			}
			
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
