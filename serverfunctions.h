//serverfunctions


void startServer(){
	int sockfd, newsock , client;
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
   serv_addr.sin_port = htons( 2525 );//defenição de porta 

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
       
      if (client_sock < 0)
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
    char *message , client_message[2000];
    char login[30];
    //Send some messages to the client
    while(n!=0 && n < 4){ //verificação de login
    	//message = "Efetue login: "; o cliente usa diretamente nos argumentos da aplicação o seu utilizador
    	//write(sock , message , strlen(message));
   		recv(sock , login , 30 , 0);
   		user_code=findUser(login);
   		if (user_code!=-1)
   		{
   			n=0;
   		}
   		else
   		{
   			message = "Falhou o login tente de novo.\n"
   			write(sock , message , strlen(message));
   			++n;
   		}
   	}

   	if (n>=4){
   	    message = "Atingiu o número máximo de logins permitidos.\n A sua ligação será desliga!\n"
   		write(sock , message , strlen(message));
   		close(sock);
   	}

   	n=1;

   	 while(n!=0 && n < 4){ //verificação de passwoard
   	 	message = "Password: "
   	 	write(sock , message , strlen(message));
   	 	recv(sock , login , 30 , 0);//nao vou inciar mais nenhuma variavel a pass vai ser igual e ja tneho o codigo d eutilizador
   	 	if (strncmp(login,Dados[user_code].password)==0)
   			break;
   		++n;
   	 }

   	 if (n>=4)//cortar a ligação ainda naos ei comos e faz
   	{
   	    message = "Atingiu o número máximo de logins permitidos.\n A sua ligação será desliga!\n"
   		write(sock , message , strlen(message));
   		close(sock);
   		
   	}

   	message = "Bem vindo, "
   	write(sock , message , strlen(message));
   	write(sock , Dados[user_code].login , strlen(Dados[user_code].login));
   	message = "\n";
   	write(sock , message , strlen(message));

   	char menu = "1 - Para uma nova mensagem \n2 - Para sair do programa\n";
   	n=0;

   	while(n!=2){
   		write(sock , menu , strlen(menu));
   		recv(sock , n , 1 , 0);
   		newsms(*socket_desc);
     /////////////////////////////////////////////////////////////////////////////////////////////////
   	}
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}

