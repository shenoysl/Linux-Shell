/****************************************************************
 * Developer Name: Satya Shenoy
 * Project Description: Building a simple unix shell with basic functionality
 * Due Date: 12/4/2021
 * Course: CMSC257
*****************************************************************/

/* $begin shellmain */
#include "csapp.h"
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
void exit_command();
int  cd_command(char **argv);
void help_command();
void pid_command();
void ppid_command();

char *default_prompt = "sh257"; /* global variable to hold default shell prompt */


/* signal handler function to ignore Control C */
void sigint_handler(int sig){

printf("\n%s> ", default_prompt);
fflush(stdout);
return;


}


int main(int argc, char **argv) 
{

signal(SIGINT, sigint_handler); /* install Ctrl C signal handler */
    char cmdline[MAXLINE]; /* Command line */


if(argc==3){

if(!strcmp(argv[1], "-p")){
default_prompt=argv[2]; /* makes the third argument of input prompt, the new prompt */ 

}

}

    while (1) {
	/* Read */
	printf("%s> ", default_prompt);                   
	Fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);

	/* Evaluate */
	eval(cmdline);
    } 
}
/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
   
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { 
        if ((pid = Fork()) == 0) {   /* Child runs user job */
            if (execvp(argv[0], argv) < 0) {
		printf("Execution failed (in fork)\n\n");
                printf("%s: Command not found.\n", argv[0]);
                exit(1);
            }
        }

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0){

		unix_error("waitfg: waitpid error");
}	
	printf("Process exited with status code %d\n", WEXITSTATUS(status)); /* Prints out the child exit status */
}
	
		else{
	    printf("%d %s", pid, cmdline);
}
    }
    return;
}


/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
	exit(0);
	  
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	return 1;  
//return 0;                     /* Not a builtin command */




	if(!strcmp(argv[0], "exit")){ /* if user enters "exit" call exit command function */
	
	exit_command();
		
	return 1;	

}
	
	else if (!strcmp(argv[0], "pid")){ /* if user enters "pid" call pid command function */
	
	pid_command();	

	return 1;	
}	
	
	else if (!strcmp(argv[0], "ppid")){ /* if user enters "ppid" call ppid command function */
	
	ppid_command();

	return 1;
}
	else if (!strcmp(argv[0], "help")){ /* if user enters "help" call help command function */
	
	help_command();

	return 1;
}
	else if (!strcmp(argv[0], "cd")){ /* if user enters "cd" call cd command function */
	
	cd_command(argv);

	return 1;
}
return 0;	
}

//Built in command function for exit, exits the shell
void exit_command(){

raise(SIGTERM);
printf("Terminated\n");




}




//Function for built in command, pid, prints the process id of the shell
void pid_command(){

int pid_t = getpid();
printf("%d\n", pid_t);


}

//Function for built in command, ppid, prints the parent process id of the shell
void ppid_command(){

int ppid_t = getppid();
printf("%d\n", ppid_t);



}

//Function for built in command, cd, prints current working directory or changes to a different directorty with cd<path>
int cd_command(char **argv){

if(argv[1]==NULL){

char cwd[100];
getcwd(cwd,100);
printf("%s\n", cwd);
return 1;
}
else if (!strcmp(argv[1],"..")) {

chdir("..");

}
else{
char file_path[100];
getcwd(file_path,100);
strcat(file_path,"/");
strcat(file_path, argv[1]);
chdir(file_path);

}
return 1;

}


//Functino for bult in command, help, gives the user useful information regarding the shell
void help_command(){

printf("************************************************************************\n");
printf("\n");
printf("A Custom Shell for CMSC 257\n");
printf("\n");
printf("- Satyanarayana Shenoy\n");
printf("\n");
printf("Usage: \n");
printf("\n");
printf("- The user can change the prompt for the shell by typing ./sh257 -p <prompt> \n");
printf("************************************************************************\n");
printf("\n\n");
printf("BUILTIN COMMANDS: \n\n");
printf("- exit: Exits the shell \n\npid: prints the process id of the shell \n\nppid: prints the parent process id of the shell \n\nhelp: provides the developer name of the shell, prinst usage information, and refers to use man for help with non built-in commands\ncd: prints the current working directory, changes current working directory when cd<path> is typed\n\n");
printf("SYSTEM COMMANDS: \n\n");
printf("Please refer to the man pages for the system commands and their functionalities\n\n");

}


/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */


