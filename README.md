# SMS Receaver
#####André Cirne - 201505860
#####Cristiana Morais - 201505454
O sistema que se encontra aqui apresentado é uma aplicação, em liguangem C, que tem como capacidade a comunicação entre vários utilizadores em diferentes localizaçoões, sendo que toda a sua comunicação é centralizada, passando por um único servidor. A tecnologia que se encontra na base do servidor são as sockets. Sockets é o ponto-final de um fluxo de comunicação entre dois programas através de uma rede. 

## 1. Instruções de instalação
### Cliente: 
#### Componentes:
* main.c
* functions.h
* header.h
* passinput.h

#### Instrução de compilação:

```sh
	$ gcc main.c -o cliente -lpthread
```

### Servidor
#### Componentes:
* Database_handler.h
* header.h
* passinput.h
* serverfunctions.h
* server.c

#### Instrução de compilação:

```sh
	$ gcc server.c -o servidor -lpthread
```

## 2. Utilização do programa

### Cliente:
Para utilização do cliente devemos chamar a função cliente com os seguintes argumentos: utilizador, ip e porta da seguinte forma, que e descrita no exemplo abaixo, no qual joao é o utilizador, 192.168.0.4 é o ip do servidor, 2524 é a porta onde se encontra o servidor:
```sh
	$ ./cliente joao@192.168.0.4:2524
```

Depois da autenticação efetuada o utilizador poderá enviar e receber mensagens, listar os utilizadores online e mudar a palavra-passe para autenticação. É de chamar atenção a possibilidade de enviar a mesma mensagem a vários utilizadores, escrevendo-os no campo para, separado por virgulas.

Ao fazer a autenticação é nos apresentado um menu disponível para o utilizador com 4 opções de utilização:

* 1) Listar utilizadores online - mostra todos os utilizadores que no momento estão online incluíndo ele próprio.

* 2) Mandar SMS para um ou mais utilizadores - permite ao utilizador autenticado enviar mensagens aos utilizadores que se encontra online.

* 3) Logout - o utilizador pode sair do programa.

* 4) Mudar password - com esta opção é possivel alterar a password de autenticação.


### Servidor:
O servidor para ser inciado normalmente basta executa-lo.
Com o servidor a correr os utilizadores poderão conectar-se sem qualquer mais demora ao servidor. Claro que na primeira utilização do servidor devera criar os utilizadores, utilizando a opção descrita abaixo. 
A porta que é utilizada por default é a porta 2524, no entanto pode ser mudada digitando -:seguido da nova porta sem espaços e no final reniciando o servidor. Outra opção é correr o servidor com os argumentos adduser, para adicionar um utilizador ou : para mudar de porta como é exemplificado a seguir.
```sh
    $ ./servidor adduser [novo utilizador]
    $ ./servidor : [nova porta]
```

 Com a inicianção do servidor fica disponivel uma consola do servidor com as opções:
 
 * -a : adicionar um novo utilizador; - permite adicionar um novo utilizador ao servidor.
 
 * -m : Enviar mensagem global; - envia uma mensagem a todos os utilizadores online.
 
 * -p [Utilizador] : mudar a passwoard de um utilizador; - possibilita a alteração da password do utilizador.
 
 * -s :mandar sms; - envia um sms a um utilizador. 
 
 * -h : mostra a ajuda à consola; - apresenta um menu de ajuda sobre a utilização da consola.
 
 * -r: eliminar utilizador ou lista de utilizadores; - elimina um ou mais utilizadores, sendo possivel tambem faze-lo de duas formas diferentes: 
 
     * -a: opção que apaga todos os utilizadores; - apaga todos os utilizadores disponíves.
     
     * -v: apagar de forma verbal; - avisa antes de apagar um utilizador.
     
 * -:[port] : altera a porta do servidor.

 * -q : desliga o servidor.
 		

## 3. Resolução de erros comuns:
#### ERRO - Bind falhou
Por norma este erro significa que a porta na qual está a tentar abrir já se encontra em uso por outro programa, poderá pará-lo ou simplesmente mudar a porta, usando a como argumento ':' espaço e a porta que pretende usar.

## 4. Possiveis questões e funcionalidades:
#### As mensagens offline mantém-se quando o servidor é reniciado?
Sim, as mensagens enquanto não forem entregues ao utilizador final não são apagadas do sistema. O sistema utiliza ficheiros de texto auxiliares para guardar mensagens, definições e utilizadores.
#### É necessário reiniciar o programa para fazer alterações como por exemplo adicionar/remover utilizadores ou alterar palavras passe?
Não, o servidor tem a capacidade de recarregar base de dados sem que seja reiniciado, levando a que todas estas alterações possam ser feitas, mantendo o normal serviço de sms's. A unica configuração na qual é necessário reiniciar o servidor é a configuração de porta.


#####Nota: Este programa poderá ser utilizado ao nível de uma unica máquina, usando o localhost como ip, nomeadamente 127.0.0.1.