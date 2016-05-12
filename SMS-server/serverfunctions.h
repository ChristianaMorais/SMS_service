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
   	exit(EXIT_FAILURE);
   }

   //Definir parametros de serv_addr
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY; //definição de ip 
   serv_addr.sin_port = htons( portReader() );//defenição de porta 

   //juntar tudo
   if( bind(sockfd,(struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0){
   	perror("ERRO - Bind falhou");
   	exit(EXIT_FAILURE);
   }
    listen(sockfd , 3); //espera por ligação o numero e o maximo de ligaçoes

      //consola de administração do servidor 
    pthread_t menu_thread;
    if( pthread_create( &menu_thread , NULL ,  serveradminpanel , NULL) < 0)
    {
    	perror("could not create thread");
    	exit(EXIT_FAILURE);
    }

    puts("Servidor iniciado.");
    puts("A espera de ligação");
    puts("Digite -q para desligar em segurança o servidor.");
    c = sizeof(struct sockaddr_in); 
    while( (client = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&c)) ){
    	pthread_t sniffer_thread;
    	newsock = malloc(4);
    	*newsock = client;

    	if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) newsock) < 0){
    		perror("could not create thread");
    		exit(EXIT_FAILURE);
    	}
    }

    if (client < 0){
    	perror("accept failed");
    	exit(EXIT_FAILURE);
    }
}


void *connection_handler(void *socket_desc) {
  
	int sock = *(int*)socket_desc, i,n=0;
	int read_size;
	int user_code=-1;
  int coderr=0; //numero de vezes que a mensagem falhou
  char message[1000] , client_message[2000], menuact[3];
  char login[30],pass[30];

  /*Autenticação do nome do utilizador*/
  //n contador de logins
  while(n<=3){ //verificação de utilizador
  	bzero(message, sizeof(message));
  	recv(sock , login , 30 , 0);
  	formatter(login);
    user_code=findUser(login); //compara o nome dado com a base de dados
    ++n;
    if (user_code!=-1){
    	bzero(message, sizeof(message));
      strcat(message,"2");//codigo para dizer que o login foi aceite
      write(sock , message , strlen(message));
      break;
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
       else{
    	    bzero(message, sizeof(message));
          strcat(message,"0"); //codigo para continuar o ciclo
          write(sock , message , strlen(message));  
      }
    }
  }


  /*Autenticação da passwoard*/
  n=0;//contador de tentaivas
  while(n<=3){
      bzero(pass, sizeof(pass));
      recv(sock , pass , 30 , 0);
      formatter(pass);
      if (strcmp(pass,Dados[user_code].password)==0) //confirma se a passwoard corresponde com o utilizador
      	break;
      ++n;
      if (n==3){//significa que exedeu  as tentativas permitidas
         	bzero(message, sizeof(message));
       	  strcat(message,"1");
       	  write(sock , message , strlen(message));
       	  puts("Ligação desligada, login sem sucesso.");
         	free(socket_desc);
       	  return 0;
       }
       else{
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
  //Identificaçao do que pretende o utilizador
  while(n!=9){
    bzero(menuact, sizeof(menuact));
   	recv(sock ,menuact , 1 , 0);//fazer verificação de digitos
   	n=menuact[0]-'0';
   		switch(n){
   			case 1:
   			  onlineusers(sock);
          coderr=0;
   			  break;
   			case 2:
   			  i=smssender(user_code,sock);
          coderr=0;
   			  break;
   			/*case 3:
        	printf("%s ficou offline.\n",Dados[user_code].login ); //ver que caso e e este 3
          break;*/
        case 9:
        	break;//o 9 como n ira parar o ciclo
        case 4:
        	remoteChanger(user_code,sock);
          coderr=0;
        break;

        default:
        	if (coderr>=3){//o o utilizador pode dar ate 3 comunicaçoes desconhecidas seguidas , doutra forma o programa fecha automaticamente
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
  char argumentos[600],user[30],corpo[500], final[100], send[600]; //user e aquele que queremos enviar
  int n=0, i=0, userSend,p=0, firstcomma;
  bzero(argumentos,sizeof(argumentos));
  bzero(final,sizeof(final));
  recv(socksender,argumentos,1024,0);

  for (i = 0; argumentos[i]!=';'; ++i);
  firstcomma=i;

  for ( ++i  ; argumentos[i] != '\0' ; ++i){//isolar a mensagem em si
    corpo[n]=argumentos[i];
    ++n;
  }
  corpo[n]='\0';
  i=0;
  /*Isola os utilizadores */
  do{
    if (argumentos[i] == ',' || argumentos[i] == ';'){
      user[p]='\0';

    if (strcmp(user,"admin") == 0){
      printf("---Nova mensagem para administrador de %s---\n%s\n\n",Dados[user_code].login,corpo);
      ++i;
      p=0;
      if (strlen(final)!=0)
        strcat(final,", ");
      strcat(final,"admin");
      continue;
    }

    userSend=findUser(user);
    if (userSend==-1){//verifica se existe o utilizador
      write(socksender,"4",30);
    }
    //escolha do metodo de envio
    bzero(send,sizeof(send));
    if (Dados[userSend].sock!=-1){//online
      strcat(send,Dados[user_code].login);
      strcat(send,";");
      strcat(send,corpo);
      strcat(send,"8");
      strcat(send,"\0");
      write(Dados[userSend].sock,send,strlen(send));
    }
    else {
      offlineSMS(user_code,userSend,corpo);     
    }
    p=0;
    if (strlen(final)!=0)
      strcat(final,", ");
    strcat(final,Dados[userSend].login);
  }
  else{
    user[p]=argumentos[i];
    ++p;
  }
    ++i;
  }while(i<=firstcomma);

  write(socksender,"2",30);
  printf("-> %s mandou uma mensagem para %s\n",Dados[user_code].login, final);
  return 0;
}

void globalSMS(){
	__fpurge(stdin);
	char message[600], v;
  bzero(message,sizeof(message));
	int i=6;
	strcat(message,"admin;");
	printf("Mensgaem global: ");

	v=getchar();
	while(v!='\n' && i<592){
		message[i]=v;
		++i;
		v=getchar();
	}
	
	__fpurge(stdin);
	message[i]='8';
	++i;
	message[i]='\0';
  puts(message);
	for (i = 0; i < UserNumber(); ++i){
		if (Dados[i].sock!=-1){
      //puts(Dados[i].login);
			write(Dados[i].sock , message , strlen(message));
		}
	}
	puts("->Mensagem global enviada.");
}

void *serveradminpanel(){
	char op[50], user[60], decisao,v,pass1[30],pass2[30];
	int i, b, codeuser, flag=0,p=0;
	while(1){
  	i=0;
		bzero(op,sizeof(op));
		v=getchar();
		while(v!='\n'&&i<48){
			op[i]=v;
			++i;
			v=getchar();
		}
		op[i]='\0';
		__fpurge(stdin); //limpar o buffer metodo especifico para c em linux
		
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
    			if (!(confirma())){
            puts("!!!!! A enviar mensagem de terminação a todos os utilizadores");
            for (i = 0; i <UserNumber(); ++i){
    				  if (Dados[i].sock!=-1)
                write(Dados[i].sock,"7",2);
            }
          puts("!!!!! Servidor terminado !!!!!\n\n");
          exit(0);
    		  }else
            puts("Cancelado!");
    		  break;

    		case 'h':
    		  printf("Manual de opções da consola do servidor:\n -a : adicionar um novo utilizador;\n -m : Enviar mensagem global;\n -p [Utilizador] : mudar a passwoard de um utilizador;\n -h : mostra a ajuda à consola;\n -r: eliminar utilizador ou lista de utilizadores;\n     -a: opção que apaga todos os utilizadores;\n     -v: apagar de forma verbal;\n -:[port] : altera a porta do servdior;\n -q : desliga o servidor;\n");
    		  break;

        case ':':
          i=2;
          b=0;
          bzero(user,sizeof(user));
          while(op[i]!='\0'){
            if (isdigit(op[i])==0){
              puts("Porta inválida!");
              break;
            }
            b=b*10+(op[i]-'0');//converte string para int
            ++i;
          }
          if (b < 1 || b > 65536){ // As porta so podem ser de 1 a 65536
            puts("Porta inválida!");
            break;
          }
          portChanger(b);//muda a porta no ficheiro d econfigurações
          printf("A porta do servidor foi alterada para a porta %d.\nPara que as alterações façam efeito reinicie o servidor.\n",b);
          break;

        case 'm':
          globalSMS();   	
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
    		  if (op[3]=='-'){
            if (op[4]=='a'){
              FILE *fx;
              fx=fopen(FX,"w");
              fclose(fx);
              DBreader();
              break;
            }
            else if (op[4]=='v'){
    				  flag=1;
    				  i=6;
    				}
    			 else
    				continue; //que e eu isti faz???? nao me lembro
          }
    		
  				bzero(user,sizeof(user));
  				b=0;

  				while(op[i]!='\0'){
  					if (op[i]!=' '){
  						user[b]=op[i];
  						++b;
  						++i;
  					}
  					if(op[i]==' '||op[i]=='\0'){
  						user[b]!='\0';

              codeuser=findUser(user);//procura o codigo do utilizador
              if (codeuser!=-1){
  						  if (flag){ //se o modo verboso estiver ativado existe uma rconfirmaçao antes d eeliminar o utilizador
  						  	printf("Deseja mesmo eliminar o utilizador %s?(s/n)",user);
  						  	if(confirma())//simples função que gere um sim ou um não
  						  		break;
  						  	printf("\n");
  						  }
				
                p=0;// ira servir como o contador que nos permitira saber em que utilizador vamos (codigo do utilizador)
  							FILE *fp = fopen(FX, "a+");
  							FILE *fpc= fopen(FOC,"w");
  							while(fgets(user,60,fp)){
  								if (codeuser!=p){
  									fputs(user,fpc);
  								}  
  								++p;
  							}
  							fclose(fp);
  							fclose(fpc);
  							remove(FX);
  							rename(FOC,FX);
  							DBreader();  
  							puts("Utilizador eliminado.");
  							++i;
  							b=0;  							
  						}
  						else
  							puts("Utilizador inexistente.");
  										
  					}                
  				}
  				flag=0;
    			printf("\n");
    			break;

    		default:
          printf("Opção não reconhecida.\n");
    			break;
     	}
    }
    else
    		puts("As opções devem começar com - ex: -h");
  }
}

int confirma(){ //esta função linda com inputs do tipo sim e nao
	char c;
	c=getchar();
	__fpurge(stdin);
	if (c=='s'){
		return 0;
	}
	else
		return 1;
}


