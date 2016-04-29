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
	printf("**MENU**\n1)Listar utilizadores online.\n2)Mandar SMS a um utilizador.\n3)Logout.\n4)Mudar password.\n");
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
		case 1://utilizadores online
			messagel[(strlen(messagel)-1)]='\0';
			printf("%s",messagel);
			mainprinter();
			break;
		case 2: //estado do envio da mensagem
			puts("Mensagem enviada com sucesso.\n");
			mainprinter();
			break;
		case 3://recepção de uma mensgaem offline
			puts("Mensagem Offline");
			messagel[(strlen(messagel)-1)]='\0';
			printf("%s\n",messagel);
			break;
		case 4: //mensagem enviada sem sucesso
			puts("Enviado sem sucesso.\n");
			mainprinter();
			break;
		case 5: //mensagem enviada sem sucesso
			puts("Password alterada com sucesso.\n");
			mainprinter();
			break;

		case 7:
			printf("!!!!! Servidor informou que irá terminar o serviço de SMS !!!!!\nLogout forçado.\n");
			close(sock);
			exit(0);
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
			printf("\n");
			printf("%s: ",user );
			printf("%s\n",corpo );
			mainprinter();
			break;
		case 9:
			puts("Log out efetuado");
			close(sock);
			exit(0);
			break;
		
		default:
			puts("Insucesso na comunicação.");
			close(sock);
			exit(1);
			break;
	}
	}
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   
    while (time(0) < retTime);             
}

void passwordConfirm(char passf[]){
	int i;
	char pass1[30], pass2[30],v;
	__fpurge(stdin);
	do{
				if (i!=0)
				{
					i=0;
					printf("As password's não correspondem volte a inserir.\n");
				}
				printf("\nPassword: ");
				v=getchar();
				while (v!='\n'&&i<29){
					pass1[i]=v;
					++i;
					v=getchar();
				}
				pass1[i]='\0';
				i=0;
				printf("Reintroduza a password: ");
				v=getchar();
				while (v!='\n'&&i<29){
					pass2[i]=v;
					++i;
					v=getchar();
				}
				pass2[i]='\0';
			}while(strcmp(pass1,pass2)!=0);

			strcpy(passf,pass1);

}

void passwoardChangerSend(int sock){
	char buffer[90];
	passwordConfirm(buffer);
	write(sock,buffer,strlen(buffer));
}