/**
author: 2016118201_권용준
datetime: 2021-10-05 11:20
description
터미널에서 출력되는 모든 소문자를 대문자로 바꾸어 출력하도록 tty driver's operation중 output카테고리에 해당하는
OLCUC attribute를 tcgetattr, tcsetattr 함수를 이용해서 변경해주는 코드이다.
**/

#include 	<stdio.h>
#include	<termios.h>
#include	<stdlib.h>

#define oops(s,x) { perror(s); exit(x); }

int main ( int ac, char *av[])
{
	struct termios info;
	char ch;

    while(1){
        if( tcgetattr(0, &info) == -1 ) //tty attribute들을 가져옴.
			oops("tcgetattr", 1);
		
		if (info.c_oflag & OLCUC){
			printf("olcuc is on.\n");
		} else
			printf("olcuc is off\n");

		printf("1. olcuc is on.\n");
		printf("2. olcuc is off.\n");
		printf("3. exit\n");
		printf("Menu : ");
		ch = getc(stdin);
        getc(stdin);

        //case에 따라 OLCUC attribute를 ON, OFF 시킨다.
        //tcsetattr 이용
		if (ch == '1'){
			info.c_oflag |= OLCUC;
			tcsetattr(0, TCSANOW, &info);
		} else if (ch == '2'){
			info.c_oflag &= ~OLCUC;
			tcsetattr(0, TCSANOW, &info); 
		} else if (ch == '3'){
			return 0;
		} else {
			printf("please select in menu.\n");
		}
    }
}