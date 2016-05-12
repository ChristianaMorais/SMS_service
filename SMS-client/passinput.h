#pragma once
#include "stdio.h"
#include "termios.h"

static struct termios old, new;

void initTermios(int echo){
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

void resetTermios(void){
  tcsetattr(0, TCSANOW, &old);
}

char getch(void) {
  char ch;
  initTermios(0);
  ch = getchar();
  resetTermios();
  if (isalnum(ch))
    printf("*");
  return ch;
}