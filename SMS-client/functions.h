
int commandrcv(char message){
	if (isdigit(message)==0)
	{
		perror("falhou a comunicação");
		exit(1);
	}
	return (message-'0');
}

int SMScreater(int sock){
	char corpo[500];
	char user[30],*message;
	printf("Para: ");
	scanf("%s",user);
	printf("\n");
	printf("Mensagem: ");
	scanf("%s",corpo);
	message="";
	strcat(message,user);
	strcat(message,";");
	strcat(message, corpo);
	strcat(message,"\0");
	write(sock,message,strlen(message));
	recv(sock,message,1024,0);
	return (message[0]-0);//confirmar se e digito
}

void SMSreceaver(int sock){
	char *newmessage; 
	char corpo[500];
	char user[30];
	int i, n=0;
	recv(sock,newmessage,strlen(newmessage),0);

	for (i = 0; newmessage[i]!=';'; ++i)
	{
		user[i]=newmessage[i];
	}

	user[i]='\0';

	for (++i; newmessage[i]!='\0'; ++i)
	{
		corpo[n]=newmessage[i];
		++n;
	}
	corpo[n]='\0';

	printf("%s:",user );
	printf("%s\n",corpo );

}