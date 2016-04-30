//serverfunctions

void *connection_handler(void *); //enunciado das funções
int smssender (int user_code,int socksender);
void *serveradminpanel();
int confirma();
void startServer(){
	int sockfd, *newsock , client, c;
	struct sockaddr_in serv_addr, cli_addr;//estrutura para guardar os edereços de ip do servidor e cliente

	// iniciar socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd == -1) { // verificação de estado da socket
   	perror("ERRO Impossivel de abrir socket");
   	exit(1);
   }

   //Definir parametros de serv_addr
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY; //definição de ip 
   serv_addr.sin_port = htons( portReader() );//defenição de porta 

   //juntar tudo
   if( bind(sockfd,(struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0){
   	perror("ERRO - Bind falhou");
   	exit(1);
   }
    listen(sockfd , 3); //espera por ligação o numero e o maximo de ligaçoes

      //consola de administração do servidor 
    pthread_t menu_thread;
    if( pthread_create( &menu_thread , NULL ,  serveradminpanel , NULL) < 0)
    {
    	perror("could not create thread");
    	exit(1);
    }

    puts("Servidor iniciado.");
    puts("A espera de ligação");
    puts("Digite -q para desligar em segurança o servidor.");
    c = sizeof(struct sockaddr_in); 
    while( (client = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&c)) )      {

    	pthread_t sniffer_thread;
    	newsock = malloc(4);
    	*newsock = client;

    	if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) newsock) < 0)
    	{
    		perror("could not create thread");
    		exit(1);
    	}

    }

    if (client < 0)
    {
    	perror("accept failed");
    	exit(1);
    }
}


void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
	int sock = *(int*)socket_desc, i,n=1;
	int read_size;
	int user_code=-1;
  int coderr=0; //numero de vezes que a mensagem falhou
  char message[1000] , client_message[2000], menuact[3];
  char login[30],pass[30];

  strcat(message,"1");
    write(sock , message , strlen(message));//o envio desta tem como objetivo sincronizar os dois servidores permitindo que as comunicações nao falhem

    /*Autenticação do nome do utilizador*/
    while(n!=0 ){ //verificação de utilizador
    	bzero(message, sizeof(message));
    	recv(sock , login , 30 , 0);
    	formatter(login);
      //puts(login);
      user_code=findUser(login); //compara o nome dado com a base de dados
      ++n;
      if (user_code!=-1)
      {
      	n=0;
      	bzero(message, sizeof(message));
        strcat(message,"2");//codigo para dizer que o login foi aceite
        write(sock , message , strlen(message));
    }
    else{
    	if (n==3){
    		bzero(message, sizeof(message));
          strcat(message,"1"); //envio de codigo para desligar a ligação
          write(sock , message , strlen(message));
          puts("Ligação desligada, login sem sucesso.");
          close(sock);
          free(socket_desc);
          return 0;
      }
      else
      {
      	bzero(message, sizeof(message));
          strcat(message,"0"); //codigo para continuar o ciclo
          write(sock , message , strlen(message));          
      }
  }
}

n=1;

    /*Autenticação da passwoard*/
while(n!=0 && n <4){ 
	bzero(pass, sizeof(pass));
	recv(sock , pass , 30 , 0);
	formatter(pass);
      if (strcmp(pass,Dados[user_code].password)==0) //confirma se a passwoard corresponde com o utilizador
      	break;
      ++n;

       if (n==4)//significa que exedeu  as tentativas permitidas
       {
       	bzero(message, sizeof(message));
       	strcat(message,"1");
       	write(sock , message , strlen(message));
       	puts("Ligação desligada, login sem sucesso.");
      //close(sock);
       	free(socket_desc);
       	return 0;
       }
       else
       {
       	bzero(message, sizeof(message));
        strcat(message,"0");//indica que nao acertou e que o ciclo vai continuar
        write(sock , message , strlen(message));
    }
}


bzero(message, sizeof(message));
strcat(message,"2");
    write(sock , message , strlen(message)); //envia o codigo que a passoward foi aceite
    socketSaver(user_code, sock);
    printf("* Utilizador %s  iniciou a sua sessão com sucesso.\n",Dados[user_code].login );
    offlineRECEIVER(sock,user_code);
    while(n!=9){
    	bzero(menuact, sizeof(menuact));
   		recv(sock ,menuact , 1 , 0);//fazer verificação de digitos
   		n=menuact[0]-'0';
   		switch(n){
   			case 1:
   			onlineusers(sock);
   			break;
   			case 2:
   			i=smssender(user_code,sock);
   			break;
   			case 3:
        	printf("%s ficou offline.\n",Dados[user_code].login ); //ver que caso e e este 3
        	break;
        	case 9:
        	break;

        case 4:
        	remoteChanger(user_code,sock);
        	break;
        	default:
        	if (coderr>=3)
        	{
        		n=9;
        	}
        	else{
        		perror("Erro na comunicação.");
        		++coderr;
        	}
        	break; // assim so vai desligar esta socket

        }

    }

    socketSaver(user_code, -1);
    logLogoff(user_code);
    printf("# %s fez logout\n",Dados[user_code].login );
    message[0]='\0';
    strcat(message,"9");
    free(socket_desc);
    write(sock , message , strlen(message));
    return 0;
}

int smssender(int user_code,int socksender){
  char argumentos[600],user[30],corpo[500]; //user e aquele que queremos enviar
  int n=0, i=0, userSend;
  bzero(argumentos,sizeof(argumentos));
  recv(socksender,argumentos,1024,0);
  for (i = 0; argumentos[i] != ';'; ++i) //isola o utilizador de quem veio
  	user[i]=argumentos[i];
  user[i]='\0';
  userSend=findUser(user);

  if (userSend==-1)//verifica se existe o utilizador
  {
    write(socksender,"4",30);//pode dar erro
    return 1;
}

  //isolar a mensagem em si
for (++i; argumentos[i] != '\0'; ++i){
	corpo[n]=argumentos[i];
	++n;
}
corpo[n]='\0';
  //escolha do metodo de envio
  //argumentos[0]='\0'; //reenicia a string 
bzero(argumentos,sizeof(argumentos));
  if (Dados[userSend].sock!=-1)//online
  {
  	strcat(argumentos,Dados[user_code].login);
  	strcat(argumentos,";");
  	strcat(argumentos,corpo);
  	strcat(argumentos,"8");
  	strcat(argumentos,"\0");
  	write(Dados[userSend].sock,argumentos,strlen(argumentos));
  }
  else
  {
    offlineSMS(user_code,userSend,corpo); //temporario em breve criar o serviço de sms offline
    //return;
}
write(socksender,"2",30);
printf("-> %s mandou uma mensagem a %s\n",Dados[user_code].login, Dados[userSend].login);
return 0;
}


void *serveradminpanel(){
	char op[50], user[60], decisao,v,pass1[30],pass2[30];
	int i, b, codeuser, flag=0,p=0;
	while(1){
   // scanf("%s",op);
		i=0;
		bzero(op,sizeof(op));
		v=getchar();
		while(v!='\n'&&i<48){
			op[i]=v;
			++i;
			v=getchar();
		}
		op[i]='\0';
		__fpurge(stdin); //limpar o buffer metdo especifico para c em linux
		if(op[0]=='-'){
    	switch(op[1]){
    		case 'p'://alterar a passwoard
    			i=3;
    			b=0;
    			bzero(user,sizeof(user));
    			while(op[i]!='\0'){
    						user[b]=op[i];
    						++i;
    						++b;
    			}
    			user[b]='\0';
    			//puts(user);
    			codeuser=findUser(user);
    			if (codeuser!=-1){
    				i=0;
    				passwordConfirm(user);
    				passwordChanger(codeuser,user);
    			}
    			else
    				puts("Utilizador inexistente.");
    			__fpurge(stdin);
    			break;

    		case 'q':
    		printf("!!!!!\nTem a certeza que pretende terminar o servidor?\nEstão neste momento %d utilizadores online...\nConfirme por favor (s/n) ? ", onlineuserscounter());
    		//decisao=getchar();
    		if (!(confirma()))
    		{
    			puts("!!!!! A enviar mensagem de terminação a todos os utilizadores");
    			for (i = 0; i <UserNumber(); ++i)
    			{
    				if (Dados[i].sock!=-1)
    					write(Dados[i].sock,"7",2);
    			}
    			puts("!!!!! Servidor terminado !!!!!\n\n");
    			exit(0);
    		}
    		break;
    		case 'h':
    		printf("Manual de opções da consola do servidor:\n -a : adicionar um novo utilizador;\n -p [Utilizador] : mudar a passwoard de um utilizador;\n -h : mostra a ajuda à consola;\n -r: eliminar utilizador ou lista de utilizadores;\n     -a: opção que apaga todos os utilizadores;\n     -v: apagar de forma verbal;\n -q : desliga o servidor;\n");
    		break;

    		case 'a':
    			printf("Novo utilizador: ");
    			scanf("%s",user);
    			__fpurge(stdin);
    			addUser(user);
    			DBreader();    			
    		break;

    		case 'r':
    		i=3;
    		if (op[3]=='-')
    		{
    			if (op[4]=='a')
    			{
    				FILE *fx;
    				fx=fopen(FX,"w");
    				fclose(fx);
    				break;
    			}
    			else if (op[4]=='v'){
    				flag=1;
    				i=6;
    				//puts("detetou v");
    			}
    			else
    				continue;
    		}
    		//else{
    				//puts("inicio bzero");
    				bzero(user,sizeof(user));
    				b=0;
    				while(op[i]!='\0'){
    					if (op[i]!=' ')
    					{
    						user[b]=op[i];
    						++b;
    						++i;
    					}
    					if(op[i]==' '||op[i]=='\0'){
    						//printf("%d\n", flag);
    						user[b]!='\0';
    						if (flag)
    						{
    							printf("Deseja mesmo eliminar o utilizador %s?(s/n)",user);
    							if(confirma())
    								break;
    							printf("\n");
    						}
    						codeuser=findUser(user);
    						if (codeuser!=-1)
    						{
    							FILE *fp = fopen(FX, "a+");
    							FILE *fpc= fopen(FOC,"w");
    							while(fgets(user,60,fp)){
    								if (codeuser!=p){
    									fputs(user,fpc);
    									//puts(user);
    								}  
    								++p;
    							}
    							fclose(fp);
    							fclose(fpc);
    							remove(FX);
    							rename(FOC,FX);
    							DBreader(); //podera conter outros nomes 
    							puts("Utilizador eliminado.");
    							++i;
    							b=0;
    							p=0;
    						}
    						else
    							puts("Utilizador inexistente.");
    										
    					}                
    				}
    				//printf("\n cara ter %c\n",op[i-1]);
    				flag=0;
    			printf("\n");
    			break;

    			default:
          	printf("Opção não reconhecida.\n");
    			break;
    		//}
    	}

    }else
    		puts("As opções devem começar com - ex: -h");
    }
}

int confirma(){
	char c;
	c=getchar();
	__fpurge(stdin);
	if (c=='s')
	{
		return 0;
	}
	else
		return 1;
}


