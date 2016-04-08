#include "header.h"

int main(int argc, char const *argv[])
{
	if (argc!=1)
	{
		if (/* condition */)
		{
			int sock;
			struct sockaddr_in server;
			char *message;
			char ipserver[10];//pode falhar por causa da terminar da strting
			int port, i;

			//incio da socket
			sock=socket(AF_INET, SOCK_STREAM, 0);
			if (sock==-1)
						{
							perror("A criação da socket falhou.")
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
				i=commandrcv(message);
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
				i=commandrcv(message[0]);
			}while(i!=1 && i!=2);

			if (i==1)
			{
				perror("Atingiu o limite de tentativas para passwoard.");
				exit(1);
			}

			while(1){


			}
			close(sock);
		}
		else
		{
			perror("Sintaxe não reconhecida.")
			exit(1);
		}
	}
	else{
		perror("Chamada do cliente sem argumentos!");
		exit(1);
	}
	return 0;
}
