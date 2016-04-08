//DataBase handler
//#include <stdio.h>

#define FX "BaseDados"

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
		char pass[29];
		int n=0;
		fx=fopen(FX,"a");
		printf("Password: ");
		s=getchar();
		while (s!='\n'&&n<29){
			pass[n]=s;
			++n;
			s=getchar();
		}
		strcat(arg,";");
		strcat(arg,pass);
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

void formatter(char login[]){
	int i;
	for (i = 0; i < 29 && isalnum(login[i])!=0; ++i);		
		login[i]='\0';
}
