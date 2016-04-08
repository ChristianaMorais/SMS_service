
int commandrcv(char message){
	if (isdigit(message[0])==0)
	{
		perror("falhou a comunicação");
		exit(1);
	}
	return (message-'0');
}