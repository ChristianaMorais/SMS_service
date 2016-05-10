
# SMS Receaver

O sistema que se encontra aqui apresentado é uma aplicação, em liguangem C, que tem como capacidade a comunicação entre vários utilizadores em diferentes localizaçoões, sendo que toda a sua comunicação é centralizada, passando por um único servidor. A tecnologia que s eencontra na base do servidor são as sockets. Sockets  

## Instruções de instalação

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

## Utilização do programa

### Cliente:
Para utilização do cliente devemos chamar a função cliente com os seguintes argumentos: utilizador, ip e porta da seguinte forma, que e descrita no exemplo abaixo, no qual joao é o utilizador, 192.168.0.4 é o ip do servidor, 2524 é a porta onde se encontra o servidor:
			./cliente joao@192.168.0.4:2524
		Depois da autenticação efetuada o utilizador poderá enviar e recebe rmensagens para qualquer utilizador que se encontre nesse servidor,
		listar os utilizadores online e mudar a palavra-passe para autenticação.

### Servidor:
O servidor para ser inciado normalmente basta executa-lo.
Com o servidor a correr os utilizadores poderão conectar-se sem qualquer mais demora ao servidor. Claro que na primeira utilização do servidor devera criar os utilizadores, para isso existem duas opções -a novoutilizador. 
A porta que é utilizada por default é a porta 2524, no entanto pode ser mudada digitando -:seguido da nova porta sem espaços e no final reniciando o servidor.
Para fechar o servidor basta digitar -q.		

## Resolução de erros comuns:



## Possiveis questões e funcionalidades: