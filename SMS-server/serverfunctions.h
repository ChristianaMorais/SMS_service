//serverfunctions

void *connection_handler(void *); //enunciado das funções
int smssender (int user_code,int socksender);

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
   serv_addr.sin_port = htons( 2524 );//defenição de porta 

   //juntar tudo
    if( bind(sockfd,(struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0){
    	perror("ERRO - Bind falhou");
    	exit(1);
    }
    listen(sockfd , 3); //espera por ligação o numero e o maximo de ligaçoes

    puts("A espera de ligação");
    c = sizeof(struct sockaddr_in); 
    while( (client = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&c)) )      {
          puts("Conecção aceite");
           
          pthread_t sniffer_thread;
          newsock = malloc(4);
          *newsock = client;
           
          if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) newsock) < 0)
          {
              perror("could not create thread");
              exit(1);
          }
           
          //Now join the thread , so that we dont terminate before the thread
          //pthread_join( sniffer_thread , NULL);
          puts("Handler assigned");
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
      puts(login);
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
    //printf("%s\n",message );
    write(sock , message , strlen(message)); //envia o codigo que a passoward foi aceite
    socketSaver(user_code, sock);
    printf("O utilizador %s encontra-se online.\n",Dados[user_code].login );
    offlineRECEIVER(sock,user_code);
    while(n!=9){
      bzero(menuact, sizeof(menuact));
   		recv(sock ,menuact , 3 , 0);//fazer verificação de digitos
      n=menuact[0]-'0';
      switch(n){
        case 1:
        	onlineusers(sock);
        	break;
        case 2:
        	i=smssender(user_code,sock);
        	break;
        case 3:
        	printf("%s ficou offline.\n",Dados[user_code].login );
       		 break;
        case 9:
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

    //Free the socket pointer

    socketSaver(user_code, -1);
    printf("%s ficou offline.\n",Dados[user_code].login );
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
    write(socksender,"-1",30);//pode dar erro
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
  return 0;
}

