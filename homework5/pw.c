/**
author: 2016118201_권용준
datetime: 2021-10-05 12:10
description
터미널에서 패스워드를 입력받을 때 터미널 드라이브 세팅에서 ECHO모드가 ON 상태라면 보안이 취약하게된다.
따라서 ECHO모드를 OFF시킨 후 입력을 받도록한다      
**/
#include 	<stdio.h>
#include	<termios.h>
#include	<stdlib.h>
#include    <string.h>

#define oops(s,x) { perror(s); exit(x); }

int main ( int ac, char *av[])
{
	struct termios info;
    char password[] = "s1234";
    char tmp[500] = {0};
    char nPassword[500] = {0};
    char rPassword[500] = {0};
    
    printf("Changing password for you.\n");
    
    if( tcgetattr(0, &info) == -1 )     /* get         attribs */
		oops("tcgetattr", 1);
    
    //ECHO 켜져있으면 끔.
    if (info.c_lflag & ECHO){
        info.c_lflag &= ~ECHO;
        tcsetattr(0, TCSANOW, &info);
    }
    
    printf("Current password : ");
    scanf("%s", tmp);
    printf("\n");
    if (strcmp(password, tmp) == 0) {
        printf("New password : ");
        scanf("%s", nPassword);
        printf("\n");
        printf("Retype new password : ");
        scanf("%s", rPassword);
        printf("\n");
        if (strcmp(nPassword, rPassword) == 0){
            printf("pw: password updated successfully\n");
        } else {
            printf("pw: retyped password is not same.\n");
        }
    } else {
        printf("pw: Authentication error\n");
        printf("pw: password unchanged\n");
    }

    //종료전 ECHO 꺼져있다면 켬.
    if (!(info.c_lflag & ECHO)){
        info.c_lflag |= ECHO;
        tcsetattr(0, TCSANOW, &info);
    }
}