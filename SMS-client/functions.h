#include "time.h"
//#define KMAG  "\x1B[35m"
int commandrcv(char messagel){
	if (isdigit(messagel)==0)
	{
		perror("A comunicação falhou.");
		exit(1);
	}
	return (messagel-'0');
}
void mainprinter(){
	printf("*****MENU******\n1)Listar os utilizadores online.\n2)Enviar nova mensagem.\n3)Log out.\n");
}

void SMScreater(int sock){
	char corpo[500];
	char c;
	int i=0;
	char user[30],messagel[600];
	printf("Para: ");
	scanf("%s",user);
	//printf("\n");
	printf("Mensagem: ");
	c=getchar(); // paara absorver o enter que anda por ai perdido
	c=getchar();
	while(c != '\n' && i < 500){
		corpo[i]=c;
		++i;
		c=getchar();
	}
	corpo[i]='\0';
	printf("\n");
	bzero(messagel,sizeof(messagel));
	strcat(messagel,user);
	strcat(messagel,";");
	strcat(messagel, corpo);
	strcat(messagel,"\0");
	write(sock,messagel,strlen(messagel));
}

void *SMSreceaver(void *socket_desc){
	int sock = (int*)socket_desc;
	//puts("fez o sock");
	char messagel[600]; 
	char corpo[500];
	char user[30];
	int i, n=0,s, b=0;
	while(1){
	bzero(messagel,sizeof(messagel));
	//puts("a espera de se ligar");
	recv(sock,messagel,600,0);
	//printf("%s entrou no sms receaver\n",messagel);
	s=messagel[(strlen(messagel)-1)]-'0';

	//puts(messagel[(strlen(messagel)-1)]);
	switch(s){
		case 1:
			messagel[(strlen(messagel)-1)]='\0';
			printf("%s",messagel);
			mainprinter();
			break;
		case 2: //estado do envio da mensagem
			puts("Mensagem enviada com sucesso.");
			mainprinter();
			break;
		case 3:
			puts("Mensagem Offline");
			messagel[(strlen(messagel)-1)]='\0';
			printf("%s",messagel);
			break;
		case 8: //recepção de mensagem normal
			messagel[(strlen(messagel)-1)]='\0';//retirar o carater de controlo
			for (i = 0; messagel[i]!=';'; ++i)
			{
				user[i]=messagel[i];
			}

			user[i]='\0';

			for (++i; messagel[i]!='\0'; ++i)
			{
				corpo[n]=messagel[i];
				++n;
			}
			corpo[n]='\0';
			n=0;
			printf("%s: ",user );
			printf("%s\n",corpo );
			mainprinter();
			break;
		case 9:
			puts("Log out efetuado");
			close(sock);
			exit(0);
			break;
		case 4: 
			puts("Enviado sem sucesso.");
			break;
		default:
			puts("Insucesso!");
			++b;
			if (b>=3)
			{
				close(sock);
				exit(0);
			}
			break;
	}
	}
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}