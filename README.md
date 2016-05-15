
# SMS Receaver

O sistema que se encontra aqui apresentado é uma aplicação, em liguangem C, que tem como capacidade a comunicação entre vários utilizadores em diferentes localizaçoões, sendo que toda a sua comunicação é centralizada, passando por um único servidor. A tecnologia que s eencontra na base do servidor são as sockets. Sockets  

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
Depois da autenticação efetuada o utilizador poderá enviar e receber mensagens para qualquer utilizador que se encontre nesse servidor,listar os utilizadores online e mudar a palavra-passe para autenticação. É de chamar atenção a possibilidade de enviar a mesma mensagem a vários utilizadores, escrevendo-os no campo para, separado por virgulas.

### Servidor:
O servidor para ser inciado normalmente basta executa-lo.
Com o servidor a correr os utilizadores poderão conectar-se sem qualquer mais demora ao servidor. Claro que na primeira utilização do servidor devera criar os utilizadores, para isso existem duas opções -a novoutilizador. 
A porta que é utilizada por default é a porta 2524, no entanto pode ser mudada digitando -:seguido da nova porta sem espaços e no final reniciando o servidor.
Para fechar o servidor basta digitar -q.		

## 3. Resolução de erros comuns:



## 4. Possiveis questões e funcionalidades:
####As mensagens offline mantém se quando o servidor é reniciado?
Sim, as mensagens enquanto não forem entregues ao utilizador final não são apagadas do sistema. O sistema utiliza ficheiros texto auxiliares pare guardar, mensagens, definições e utilizadores.

 