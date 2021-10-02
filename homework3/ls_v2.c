#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

void do_ls_no_sort(char dirname []);
void do_ls(char dirname[], int (*cmpFunc) (const void *, const void *));
int cmp(const void * a, const void * b);
int cmp_reverse(const void * a, const void * b);

void main(int ac, char * av[]) {
    
    int opt;
    
    while ((opt = getopt(ac, av, "rp")) != -1) {
        switch (opt){
            case 'r':
                if (strcmp("-r", av[1]) == 0)
                    do_ls(".", cmp_reverse);
                else{
                    ac--;
                    while( --ac ){
                        printf("%s:\n", *++av );
                        do_ls(*av, cmp_reverse);
                    }
                }
                return;
            case 'p':
                if (strcmp("-p", av[1]) == 0)
                    do_ls_no_sort(".");
                else{
                    ac--;
                    while( --ac ){
                        printf("%s:\n", *++av );
                        do_ls_no_sort(*av);
                    }
                }                
                return;
        }
    }
    if ( ac == 1 ){
        do_ls(".", cmp);
    } else
        while( --ac ) {
            printf("%s:\n", *++av );
            do_ls(*av, cmp);
        }
}

void do_ls(char dirname[], int (*cmpFunc) (const void *, const void *)) {
	DIR		* dir_ptr;
	/* Directory */
	struct dirent	* direntp;
	/* Entry */
	struct stat	info;
	int		mEnd = 0;
	int		mStart = 0;
	char	asFileName[255][255];
    
	if ( (dir_ptr = opendir(dirname)) == NULL ) {
		fprintf(stderr, "ls5: cannot open %s\n", dirname);
	} else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
			sprintf(asFileName[mEnd++], "%s", direntp->d_name);
		}
        
		closedir(dir_ptr);
		qsort(asFileName, mEnd, sizeof(asFileName[0]), cmpFunc);

        for (mStart = 0; mStart < mEnd; mStart++) {
            printf("%s\n", asFileName[mStart]);
        }
	}
}

void do_ls_no_sort(char dirname[]){
	DIR		* dir_ptr;
	/* Directory */
	struct dirent	* direntp;
	/* Entry */
	struct stat	info;
	int		mEnd = 0;
	int		mStart = 0;
	char	asFileName[255][255];
    
	if ( (dir_ptr = opendir(dirname)) == NULL ) {
		fprintf(stderr, "ls5: cannot open %s\n", dirname);
	} else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
			sprintf(asFileName[mEnd++], "%s", direntp->d_name);
		}
        
		closedir(dir_ptr);

        for (mStart = 0; mStart < mEnd; mStart++) {
            printf("%s\n", asFileName[mStart]);
        }
	}
}
int cmp(const void * a, const void * b) {
	return strcmp((char*)a, (char*)b);
}

int cmp_reverse(const void * a, const void * b){
    int tmp = strcmp((char*)a, (char*)b);
    
    if (tmp > 0)
        return -1;
    else if (tmp < 0)
        return 1;
    else
        return 0;
}