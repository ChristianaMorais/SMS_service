#include "time.h"
int commandrcv(char messagel){
	if (isdigit(messagel)==0)
	{
		perror("falhou a comunicação");
		exit(1);
	}
	return (messagel-'0');
}

int SMScreater(int sock){
	char corpo[500];
	char user[30],messagel[600];
	printf("Para: ");
	scanf("%s",user);
	printf("\n");
	printf("Mensagem: ");
	scanf("%s",corpo);
	bzero(messagel,sizeof(messagel));
	strcat(messagel,user);
	strcat(messagel,";");
	strcat(messagel, corpo);
	strcat(messagel,"\0");
	write(sock,messagel,strlen(messagel));
	bzero(messagel,sizeof(messagel));
	recv(sock,messagel,600,0);
	return (messagel[0]-0);//confirmar se e digito
}

void *SMSreceaver(void *socket_desc){
	int sock = (int*)socket_desc;
	//puts("fez o sock");
	char messagel[600]; 
	char corpo[500];
	char user[30];
	int i, n=0,s;
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
			break;
		case 2:
			puts("Mensagem enviada com sucesso.");
			break;
		case 8:
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
			break;
		case 9:
			puts("Log out efetuado");
			close(sock);
			exit(0);
		default:
			puts("Insucesso!");

	}
	}
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}