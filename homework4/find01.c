
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

void do_ls(char dirname[], char strBuff[], int depth);
int cmp(const void * a, const void * b);
int is_dir( char *fname );

int main (int ac, char *av[])
{
    char strBuff[255] = {0};
    
    if( ac == 1)
		do_ls(".", strBuff, 0);
    else
		do_ls(av[1], strBuff, 0);
}

void do_ls(char dirname[], char strBuff[], int depth) {
	DIR		* dir_ptr;
	/* Directory */
	struct dirent	* direntp;
	/* Entry */
	struct stat	info;
	int mEnd = 0;
	char asFileName[255][255];
    int cnt = 0;
    
	if ( (dir_ptr = opendir(dirname)) == NULL ) {
	} else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
			sprintf(asFileName[mEnd++], "%s", direntp->d_name);
            cnt++;
		}
        
		closedir(dir_ptr);
        
        //printf("cnt : %d\n", cnt);
        //depth가 0일 때 빈폴더 find한 경우
        if (is_dir(dirname) == 0 && depth == 0 && cnt == 2){
            printf("%s\n", dirname);
            return;
        }
        
		qsort(asFileName, mEnd, sizeof(asFileName[0]), cmp);
        
        if (depth == 0) {
            printf("%s\n", dirname);
            strncat(strBuff, dirname, strlen(dirname));
            strncat(strBuff, "/", sizeof(1));
            for (int i = 0; i < mEnd; i++){
                if (strcmp(asFileName[i], ".") == 0 || strcmp(asFileName[i], "..") == 0)
                    continue;
                char tmp_path[255] = {0};
                strncat(tmp_path, strBuff, strlen(strBuff));
                strncat(tmp_path, asFileName[i], strlen(asFileName[i]));
                if (is_dir(tmp_path) == 1){
                    printf("%s%s\n", strBuff, asFileName[i]);
                } else if (is_dir(tmp_path) == 0){
                    printf("%s%s\n", strBuff, asFileName[i]);
                    char tmp[255] = {0};
                    strncat(tmp, strBuff, strlen(strBuff));
                    strncat(tmp, asFileName[i], strlen(asFileName[i]));
                    //printf("tmp : %s\n", tmp);
                    do_ls(tmp, tmp, depth+1);
                }
            }
        } else { //depth가 1이상일 경우 
            if (cnt == 2){
                return;
            }
            
            for (int i = 0; i < mEnd; i++){
                if (strcmp(asFileName[i], ".") == 0 || strcmp(asFileName[i], "..") == 0)
                    continue;
                
                //이전에서 넘겨준 경로에 현재 파일 이름 덭붙여줌.
                char tmp[255] = {0};
                strncat(tmp, strBuff, strlen(strBuff));
                strncat(tmp, "/", sizeof(1));
                strncat(tmp, asFileName[i], strlen(asFileName[i]));
                //printf("depth >= 1 , asFileName : %s\n", asFileName[i]);
                //printf("depth >= 1 , tmp : %s\n", tmp);
                if (is_dir(tmp) == 1){
                    printf("%s\n", tmp);
                } else if (is_dir(tmp) == 0){
                    printf("%s\n", tmp);
                    char tmp2[255] = {0};
                    strncat(tmp2, tmp, strlen(tmp));
                    do_ls(tmp2, tmp2, depth+1);
                }
            }
        }
	}
}

int cmp(const void * a, const void * b) {
	return strcmp((char*)a, (char*)b);
}

 int is_dir( char *fname ){
 
     int result = 0;
 
     struct stat info;
     if (stat (fname, &info) == -1 ) {
         perror("stat");
         return -2;
     }
 
     if (S_ISREG(info.st_mode))
         return 1;
     else if (S_ISDIR(info.st_mode))
         return 0;
     else
         return -1;
 }
