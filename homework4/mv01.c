#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

 int is_dir(char *fname);

int main(int ac, char *av[]){
    
    char strBuff[500] = {0};
    
    if (ac < 3){
        printf("invalid parameter.\n");
        exit(1);
    }
    
    if (is_dir(av[2]) == 0){
        strncat(strBuff, av[2], strlen(av[2]));
        strncat(strBuff, "/", sizeof(1));
        strncat(strBuff, av[1], strlen(av[1]));
        rename(av[1], strBuff);
    } else if (is_dir(av[2]) == 1){
        rename(av[1], av[2]);
    } else {
        printf("unknown error. plz try again\n");
    }
    return 0;
}

 int is_dir( char *fname ){
 
     int result = 0;
 
     struct stat info;
     if (stat (fname, &info) == -1 ) {
         return 1;
     }
 
     if (S_ISREG(info.st_mode))
         return 1;
     else if (S_ISDIR(info.st_mode))
         return 0;
     else
         return -1;
 }