//DataBase handler
//#include <stdio.h>

#define FX "BaseDados"
#define FO "offlinemessages"
#define FOC "messageoffline"

typedef struct { //definição do novo tipo utilizador
	char login[30];
	char password[30];
	int sock; //usar *(int*) para guaradar
} utilizador;

void DBcreator(){
    FILE *fp = fopen(FX, "ab+");
    fclose(fp);
}

int UserNumber(){ //vai abrir a base de dados e ver quantos utilizadores existem
	DBcreator();
	FILE *fx; 
	char s[60];
	int l=0;//contador das linhas
	fx=fopen(FX,"r");

	while (fgets (s, 60, fx)!=NULL){
		++l;
	}
	fclose(fx);
	//puts(l);
	if (l==0)
	{
		return 1;
		perror("A sua base de dados, encontra-se vazia, se ainda nao utilizou a função adduser e aconcelhavel que o faça.");
	}
	return l;
}

utilizador Dados[30]; //temporariamente os dados tem um limite

void addUser(char arg[]){//adiciona utilizador fornecido pelo argumento
	if (UserNumber()>30)
	{
		perror("Base de dados cheia.");
		exit(1);
	}

	if (strlen(arg)>29){//medida de segurança
		perror("Nome de utilizador com carateres a mais!");
	}
	else{
		FILE *fx; 
		char s;
		char pass1[29], pass2[29];
		int n=0;
		fx=fopen(FX,"a");
		do{
			if (n!=0)
			{
				n=0;
				printf("As passwod's não correspondem volte a inserir.\n");
			}
			printf("Password: ");
			s=getchar();
			while (s!='\n'&&n<29){
				pass1[n]=s;
				++n;
				s=getchar();
			}
			pass1[n]='\0';
			n=0;
			printf("Reintroduza a password: ");
			s=getchar();
			while (s!='\n'&&n<29){
				pass2[n]=s;
				++n;
				s=getchar();
			}
			pass2[n]='\0';
		}while(strcmp(pass1,pass2)!=0);
		strcat(arg,";");
		strcat(arg,pass1);
		strcat(arg,";\n");
	//printf("%s\n",arg);
		fputs(arg,fx);
		fclose(fx);		
	}
}

void DBreader(){ //leitura de base de dados
	FILE *fx; 
	char s[60];
	int n=0,i=0,j=0;
	fx=fopen(FX,"r");
	while(fgets(s,60,fx)){
		j=0;
		for(i=0;s[i]!=';';++i) {
			Dados[n].login[j]=s[i];
			++j;
		}
		Dados[n].login[i]='\0';
		j=0;
		for(++i;s[i]!=';';++i) {
			Dados[n].password[j]=s[i];
			++j;
		}
		Dados[n].password[i]='\0';
		Dados[n].sock=-1;
		//printf("%s\n",Dados[n].login );
		//printf("%s %d\n",Dados[n].password,UserNumber() );
		++n;
	}
	fclose(fx);
}

int findUser(char login[]){ //na base de dados procura se existe um utilizador com o login inserido
	int i;
	//printf("%s ff\n",login );
	for (i = 0; i < UserNumber(); ++i)	{
		if (strcmp(login,Dados[i].login)==0)	{
			//printf("%s\n",Dados[i].login);
			return i;
		}
	}
	return -1;

}

void socketSaver(int code, int socket){
	Dados[code].sock=socket;
}

void formatter(char login[]){
	int i;
	for (i = 0; i < 29 && isalnum(login[i])!=0; ++i);		
		login[i]='\0';
}

void onlineusers(int sock){
	char message[500]="Urilizadores online:\n";
	int i, flag=0;
	for (i = 0; i < UserNumber(); ++i)	{
		if (Dados[i].sock!=-1)	{
			strcat(message,Dados[i].login);
			strcat(message,"\n");
			flag=1;
		}
	}
	strcat(message,"1");
	strcat(message,"\0");
	if (flag==1){
		printf("mensagem %s \n",message );
		write(sock,message,strlen(message));
	}
	else //se ninguem esta online comoe stas a a pedir
	{
		char message1[]="Não se encontram utilizadores online.1\n";
		write(sock,message1,strlen(message1));
	}
}

int onlineuserscounter(){
	int count=0,i;
	for (i = 0; i < UserNumber(); ++i)
	{
		if (Dados[i].sock!=-1)
			++count;
	}
	return count;
}

/*int numberChar(int numero){
	return (floor (log10 (abs (number))) + 1);
}*/

void offlineSMS(int user,int userSend,char corpo[]){ //guarda a sms num ficheiro
	FILE *fp = fopen(FO, "a");
	char message1[1000];
	char str[15];
	sprintf(str, "%d", userSend);
	bzero(message1,sizeof(message1));
	strcat(message1,str);
	strcat(message1,";");
	sprintf(str, "%d", user);
	strcat(message1,str);
	strcat(message1,";");
	strcat(message1,corpo);
	strcat(message1,";\n");
	fputs(message1,fp);//destinatario;remetente;mesnagem;
	fclose(fp);
}

void offlineRECEIVER(int sock,int codeuser){
	FILE *fp = fopen(FO, "a+");
	FILE *fpc= fopen(FOC,"w");
	char message1[1000];
	char arg[1000];
	int code=0,n=0,i=0;
	while(fgets(message1,1000,fp)){
		for (i = 0; message1[i]!=';' ; ++i)
			code=code*10+(message1[i]-'0');
		if (codeuser==code)
		{
			++i;
			code=0;
			for ( ; message1[i]!=';'; ++i)
				code=code*10+(message1[i]-'0');
			strcat(arg,Dados[code].login);
			strcat(arg," enviou: ");
			n=strlen(arg);
			++i;
			for ( ; message1[i]!=';'; ++i){
				arg[n]=message1[i];
				++n;
			}
			strcat(arg,"\n3\0");
			write(sock,arg,strlen(arg));	
		}
		else{
			fputs(message1,fpc); //coloca a frase na copia
		}
		code=0;
	}
	fclose(fp);
	fclose(fpc);
	remove(FO);
	rename(FOC,FO);
}

