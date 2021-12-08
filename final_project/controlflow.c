/* controlflow.c
 *
 * "if" processing is done with two state variables
 *	if_state and if_result
 *
 * author: 2016118201_권용준
 * datetime : 2021-11-14-22:01
 * description : else statement 처리를 위해 states에 ELSE_BLOCK을 추가했습니다. 또한 is_control_command에 else명령어 처리를 위해 조건문을 추가했고, do_control_command()에서 else 처리 조건문을 추가하였습니다. else 조건문에서는 if_state가 THEN_BLOCK일 경우에만 if_state를 ELSE_BLOCK으로 변경하도록 했습니다. 그 후 process.c에 ok_to_execute()에서 ELSE_BLOCK에서 실행가능한 명령어가 존재하는지 확인 후 실행가능한 명령어가 있다면 execute()로 명령어를 실행하도록 만듭니다.
 *
 *
 */

#include	<stdio.h>
#include	<string.h>
#include	"smsh.h"

enum states	{ NEUTRAL, WANT_THEN, THEN_BLOCK, ELSE_BLOCK};
enum results	{ SUCCESS, FAIL };

static int if_state	= NEUTRAL;
static int if_result	= SUCCESS;
static int last_stat	= 0;

int	syn_err(char *);
int process( char **args );
int ok_to_execute()
/*
 * purpose: determine the shell should execute a command
 * returns: 1 for yes, 0 for no
 * details: if in THEN_BLOCK and if_result was SUCCESS then yes
 *	    if in THEN_BLOCK and if_result was FAIL    then no
 *	    if in WANT_THEN  then syntax error (sh is different)
 */
{
	int	rv = 1; 	/* default is positive */
	
	if( if_state == WANT_THEN ) {
		syn_err("then expected");
		rv = 0;
	}
	else if ( if_state == THEN_BLOCK && if_result == SUCCESS )
		rv = 1;
	else if ( if_state == THEN_BLOCK && if_result == FAIL )
		rv = 0;
    else if ( if_state == ELSE_BLOCK && if_result == SUCCESS )
        rv = 0;
    else if ( if_state == ELSE_BLOCK && if_result == FAIL)
        rv = 1;
	return rv;
}

int is_control_command(char *s)
/*
 * purpose: boolean to report if the command is a shell control command
 * returns: 0 or 1
 */
{
	return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 || strcmp(s, "else") == 0 || 
					strcmp(s, "fi") == 0);
}

int do_control_command(char **args)
/*
 * purpose: Process "if", "then", "fi" - change state or detect error
 * returns: 0 if ok, -1 for syntax error
 */
{
	char	*cmd = args[0];
	int	rv = -1;

	if( strcmp(cmd, "if") == 0) {
		if( if_state != NEUTRAL )
			rv = syn_err( "if unexpected");
		else {
			last_stat = process( args + 1 );
			if_result = ( last_stat == 0 ? SUCCESS : FAIL );
			if_state = WANT_THEN;
			rv = 0;
		}
	}
	else if( strcmp( cmd, "then" ) == 0 ) {
		if( if_state != WANT_THEN )
			rv = syn_err( "then unexpected");
		else {
			if_state = THEN_BLOCK;
			rv = 0;
		}
	} // else는 있어도 되고 없어도 된다. 즉 THEN_BLOCK에서 ELSE 만나면 -> ELSE_BLOCK 없으면 NEUTRAL
    else if (strcmp( cmd, "else") == 0 ) {
        if (if_state != THEN_BLOCK)
            rv = syn_err("then unexpected <else stat> is not in then>");
        else {
            if_state = ELSE_BLOCK;
            rv = 0;
        }
    }
	else if( strcmp(cmd, "fi" ) == 0 ) {
		if ( if_state == THEN_BLOCK || if_state == ELSE_BLOCK ) {
            if_state = NEUTRAL;
            rv = 0;
        }
		else {
            rv = syn_err( "fi unexpected" );
		}
	}
	else
		fatal( "internal error processing:", cmd, 2);
	return rv;
}

int syn_err(char *msg)
/* purpose: handles syntax errors in control structures
 * details: resets state to NEUTRAL
 * returns: -1 in interractive mode. Should call fatal in scripts
 */
{
	if_state = NEUTRAL;
	fprintf(stderr, "syntax error: %s\n", msg);
	return -1;
}

