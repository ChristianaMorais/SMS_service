//DataBase handler
//#include <stdio.h>
#include "stdlib.h"
#define FX "BaseDados"
#define FO "offlinemessages"
#define FOC "messageoffline"

#define FC "config" //configuração
#define FON "online" //utilizadores online

int findUser(char login[]);
int charpermited(char c);

typedef struct { //definição do novo tipo utilizador
	char login[30];
	char password[30];
	int sock; //usar *(int*) para guaradar
} utilizador;

void DBcreator(){
    FILE *fp = fopen(FX, "ab+");
    fclose(fp);
}

void onlineLogreset(){//faz reset ao ficheiro de log de utilizadores online
	FILE *fp = fopen(FON, "w");
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
	/*if (l==0)
	{
		return 1;
		perror("A sua base de dados, encontra-se vazia, se ainda nao utilizou a função adduser e aconcelhavel que o faça.");
	}*/
	return l;
}

utilizador Dados[30]; //temporariamente os dados tem um limite

void passwordConfirm(char passf[]){
	int i=0;
	char pass1[30], pass2[30],v;
	do{
				if (i!=0)
				{
					i=0;
					printf("As password's não correspondem volte a inserir.\n");
				}
				__fpurge(stdin);
				printf("Password: ");
				v=getch();
				while (v!='\n'&&i<29){
					pass1[i]=v;
					++i;
					v=getch();
				}
				pass1[i]='\0';
				printf("\n");
				i=0;
				printf("Reintroduza a password: ");
				v=getch();
				while (v!='\n'&&i<29){
					pass2[i]=v;
					++i;
					v=getch();
				}
				pass2[i]='\0';
				printf("\n");
			}while(strcmp(pass1,pass2)!=0);
			//bzero(passf,sizeof(passf));
			strcpy(passf,pass1);

}

void addUser(char arg[]){//adiciona utilizador fornecido pelo argumento
	__fpurge(stdin);
	int i;
	if (UserNumber()>30)
	{
		perror("Base de dados cheia.");
		exit(EXIT_FAILURE);
	}

	if (strlen(arg)>29){//medida de segurança
		perror("Nome de utilizador com carateres a mais!");
	}

	for (i = 0; arg[i]!='\0'; ++i){
		if (charpermited(arg[i])){
			puts("Nome inválido!");
			return;
		}
	}
	//puts(arg);
	i=findUser(arg);
	if (i!=-1){
		puts("Nome de utilizador já existente.");
		return;
	}
	else{
		char pass1[29];
		passwordConfirm(pass1);
		FILE *fx=fopen(FX,"a");
		strcat(arg,";");
		strcat(arg,pass1);
		strcat(arg,";\n");
	//printf("%s\n",arg);
		fputs(arg,fx);
		fclose(fx);
		for ( i = 0; arg[i] != ';'; ++i);
    				arg[i]='\0';
    			printf("Utilizador %s foi adicionado.\n",arg);		
	}
}

void logReader(){ //leitura do ficheiro de registo de utilizadores online
	FILE *fp = fopen(FON, "r");
	int sock=0, user=0,i;
	char s[60];
	while(fgets(s,60,fp)){
		for (i = 0; s[i]!=';'; ++i)//o código do utilizador esta guardado na base
			user=user*10+(s[i]-'0');//conversao de string para integer
		++i;
		for ( ; s[i]!=';'; ++i)//leitura da socket
			sock=sock*10+(s[i]-'0');
		Dados[user].sock=sock;//socket guardada na base de dados
	user=0;
	sock=0;
	}
	fclose(fp);
}

void DBreader(){ //leitura da base de dados de utilizadores
	FILE *fx; 
	bzero(Dados,sizeof(Dados));
	char s[100];
	int n=0,i=0,j=0;
	fx=fopen(FX,"r");
	while(fgets(s,100,fx)){
		j=0;
		for(i=0;s[i]!=';';++i) {//nome do utilizador
			Dados[n].login[j]=s[i];
			++j;
		}
		Dados[n].login[i]='\0';

		j=0;
		for(++i;s[i]!=';';++i) {//passwoard
			Dados[n].password[j]=s[i];
			++j;
		}
		Dados[n].password[i]='\0';

		Dados[n].sock=-1;//inicia o utilizador como offline
		++n;
	}
	fclose(fx);
	logReader();//leitura de utilizadores online
	/*Para o caso em que o DBreader é corrido com o programa em funcionamento existe um ficheiro auxiliar que é criado, onde são guardadas as sockets dos utilizadores que se encontravam online*/
}

int findUser(char login[]){ //na base de dados procura se existe um utilizador com o login inserido
	int i;
	for (i = 0; i < UserNumber(); ++i)	{
		if (strcmp(login,Dados[i].login)==0)	{
			return i;
		}
	}
	return -1;
}

void logcreator(int code, int socket){ //guarda a sockets e o codigo dos utilizadores, para que qd recarreguemos a base de dados nao percamos os utilizadores online
	FILE *fp = fopen(FON, "a");
	char buffer[60], aux[50];
	bzero(buffer,sizeof(buffer));
	bzero(aux,sizeof(aux));
	snprintf(buffer, 60,"%d",code);
	strcat(buffer,";");
	snprintf(aux, 50,"%d",socket);
	strcat(buffer,aux);
	strcat(buffer,";\n");
	fputs(buffer,fp);
    fclose(fp);
}

void socketSaver(int code, int socket){ //muda o estado do utilizador offline -1, online socket
	Dados[code].sock=socket;
	if (socket!=-1)
		logcreator(code,socket);
}

void onlineusers(int sock){ //imprime no cliente os utilizadores online
	char message[500]="\nUtilizadores online:\n";
	int i, flag=0;
	for (i = 0; i < UserNumber(); ++i)	{
		if (Dados[i].sock!=-1)	{
			strcat(message," * ");
			strcat(message,Dados[i].login);
			strcat(message,"\n");
			flag=1;
		}
	}
	strcat(message,"\n");
	strcat(message,"1");
	strcat(message,"\0");
	if (flag==1){
		write(sock,message,strlen(message));
	}
}

int onlineuserscounter(){ //conta utilizadores online
	int count=0,i;
	for (i = 0; i < UserNumber(); ++i)
	{
		if (Dados[i].sock!=-1)
			++count;
	}
	return count;
}

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

void offlineRECEIVER(int sock,int codeuser){ // le da base de dados e envia as mensagens offlines
	FILE *fp = fopen(FO, "a+");
	FILE *fpc= fopen(FOC,"w");
	char message1[1000];
	char arg[1000];
	int code=0,n=0,i=0;
	bzero(arg,sizeof(arg));
	while(fgets(message1,1000,fp)){ //leitura de ficheiro
		for (i = 0; message1[i]!=';' ; ++i)
			code=code*10+(message1[i]-'0');
		if (codeuser==code){
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
			strcat(arg,"\n3");
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
	rename(FOC,FO); //retira as mensagens que ja foram entregues
}


void passwordChanger(int userCode, char password[]){ //mudança de password
	int i=0,p, c;
	char buffer[60];
	FILE *fp = fopen(FX, "a+");
	FILE *fpc= fopen(FOC,"w");

	while(fgets(buffer,60,fp)){
		if (userCode!=i){
			fputs(buffer,fpc);
		}
		else{
			for (p = 0; buffer[p] != ';' ; ++p);
			++p;
			for (c = 0 ; password[c] != '\0'; ++c)
			{
				buffer[p]=password[c];
				++p;
			}
			buffer[p]=';';
			++p;
			buffer[p]='\n';
			++p;
			buffer[p]='\0';
			fputs(buffer,fpc);						
		}  
		++i;
	}
	fclose(fp);
	fclose(fpc);
	remove(FX);
	rename(FOC,FX);
	DBreader();
	printf("A paswoard de %s foi alterada.\n",Dados[userCode].login);
}

void remoteChanger(int user_code,int sock){ //mudança de password remota incitada pelo utilizador
	char buffer[90];
	bzero(buffer,sizeof(buffer));
	recv(sock ,buffer , 90 , 0);
	//puts(buffer);
	passwordChanger(user_code,buffer);
	write(sock,"5",30);
}

void logLogoff(int user_code){//retira o registo de online do ficheiro 
	FILE *fpc= fopen(FOC,"w");
	FILE *fp = fopen(FON,"ab+");
	char buffer[60];
	int target=0,i;
	while(fgets(buffer,60,fp)){
		for (i = 0; buffer[i]!=';'; ++i)
			target=target*10+(buffer[i]-'0');
		if (user_code!=target)
			fputs(buffer,fpc);
		target=0;
	}
	fclose(fp);
	fclose(fpc);
	remove(FON);
	rename(FOC,FON);
}

int portReader(){ //le as configurações de porta
	int i=2524,c=0;
	char buffer[10];
	FILE *fp = fopen(FC,"ab+");
	while(fgets(buffer,10,fp) && c!=1){
		i=0;
		for (c = 0; buffer[c]!='\n' && buffer[c]!='\0'; ++c){
			i=i*10+(buffer[c]-'0');
		}
		c=1;
	}
	fclose(fp);
	return i;
}

void portChanger(int port){ //escreve as defenições de porta na configuração
	char buffer[60];
	FILE *fp = fopen(FC,"ab+");
	FILE *fpc= fopen(FOC,"w");
	snprintf(buffer, 60,"%d",port);
	fputs(buffer,fpc);
	fputs("\n",fpc);
	fgets(buffer,60,fp);//linha da porta nao serve para nada
	while(fgets(buffer,60,fp))
		fputs(buffer,fpc);
	fclose(fp);
	fclose(fpc);
	remove(FC);
	rename(FOC,FC);
}

int charpermited(char c){
	//a funçao retorna 1 se o carater nao for permitido isto pode ativar um ciclo
	switch (c){
		case ';':puts("Carater não permitido!");return 1;break;
		case ' ':puts("Carater não permitido!");return 1; break;
		default:return 0; break;
	}
}

int onlineuserChecker(int code){
	if (Dados[code].sock!=-1)
		return 1;//utilizador encontra-se online
	else
		return 0;//utilizador encontrase offline
}