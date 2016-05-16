#pragma once //para compilação de bibliotecas ser unica

//cores
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

#include "stdio.h"
#include "stdio_ext.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "string.h"
#include "ctype.h"
#include "unistd.h"    //write
#include "pthread.h" //ao executar no final colocar a bibioleta -lpthread
#include "passinput.h"
//Funções a adicionar
#include "Database_handler.h"
#include "serverfunctions.h"

