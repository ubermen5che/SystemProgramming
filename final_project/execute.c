/* execute.c - code used by small shell to execute commands */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include	<time.h>	//시간 측정을 위한 헤더파일
#include	<sys/time.h>

int child_pid;
struct timeval  tv;
double begin, end;
double res;

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
	int pid;
	int child_info = -1;
	int exe_time = 0;
	void alarm_handler();
	int set_ticker();
    char c;
    
	if( argv[0] == NULL )		/* nothing succeeds */
		return 0;
	
	printf("최소 실행 시간을 입력해주세요.\n");
	scanf("%d", &exe_time);
    fflush(stdin);
    
	if ( set_ticker(exe_time) == -1 )
        perror("set_ticker");
    
    gettimeofday(&tv, NULL);
	begin = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    
	if( (pid = fork()) == -1 )
		perror( "fork" );
	else if( pid == 0 ){
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
	}
	else{
        child_pid = pid;
        signal(SIGALRM, alarm_handler);
		if( wait(&child_info) == -1 )
			perror("wait");
		
		if (child_info == SIGKILL) {
			gettimeofday(&tv, NULL);
	        end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
            printf("start : %lf\nend : %lf\nres : %lf\n", begin, end, (double)(end - begin));
			res = (double)(end - begin) / 1000;
			printf("입력한 시간내에 명령어가 실행되지 않았습니다.\n");
			printf("수행시간 : %.3lf \n", res);
		} else if (child_info == 0) {
            signal(SIGALRM, SIG_IGN);
            gettimeofday(&tv, NULL);
            end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
            res = (double)(end - begin) / 1000;
            printf("입력한 시간내에 명령어가 정상적으로 실행되었습니다.\n");
            printf("수행시간 : %.3lf \n", res);
        }
	}

	return child_info;
}

//SIGALRM 발생시 수행할 핸들러.
// 
void alarm_handler(int signum) {
    kill(child_pid, SIGKILL);
}

int set_ticker(int n_msecs) {

	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;
    new_timeset.it_interval.tv_sec = 0;
    new_timeset.it_interval.tv_usec = 0;
    
	return setitimer(0, &new_timeset, NULL);
}