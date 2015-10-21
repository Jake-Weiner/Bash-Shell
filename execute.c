#include "mysh.h"
/*
   CITS2002 Project 2 2015
   Name(s):		student-name1 (, student-name2)
   Student number(s):	student-number-1 (, student-number-2)
   Date:		date-of-submission
 */

// -------------------------------------------------------------------

//  THIS FUNCTION SHOULD TRAVERSE THE COMMAND-TREE and EXECUTE THE COMMANDS
//  THAT IT HOLDS, RETURNING THE APPROPRIATE EXIT-STATUS.
//  READ print_cmdtree0() IN globals.c TO SEE HOW TO TRAVERSE THE COMMAND-TREE

int execute_cmdtree(CMDTREE *t)
{
    int  exitstatus =0;

    if(t == NULL)
    {			// hmmmm, a that's problem
	exitstatus	= EXIT_FAILURE;
    }
    else { 
        
    if(strncmp (t->argv[0] , "exit" , 4) == 0) //exit command
        {
            if (t->argc == 1)
            exit(exitstatus);
            else
            {exitstatus = atoi (t->argv[1]);
                printf("%d",exitstatus);
            exit(exitstatus);
            }
        }
       
            int  pid;
            
            switch (pid = fork()) {
                case -1 :
                    perror("fork() failed");     // process creation failed
                    exit(1);
                    break;
                    
                case 0:// new child process
                    if(strncmp (t->argv[0] , "cd" , 2) == 0) // if the command is cd then follow these conditions
                    {
                        if(t->argc == 1) // No path is specified so the directory is changed to the default path HOME
                        {
                            chdir (HOME);
                            perror("change directory fails");
                            exit(EXIT_FAILURE);
                        }
                     else  if(t->argc == 2 && (strchr(t->argv[1],'/')) == NULL ) // Given a path without '/'
                        {
                            chdir (t->argv[1]);
                            perror("change directory fails");
                            exit(EXIT_FAILURE);
                        }
                        else                     // Given a full path
                        {
                            chdir (t->argv[1]);
                            perror("change directory fails");
                            exit(EXIT_FAILURE);
                        }
                    }
                     if ((strchr(t->argv[0],'/')) == NULL )  // normal command without '/'
                    {
                    execvp(t->argv[0],t->argv);
                    exit(EXIT_FAILURE);
                    }
                    else                      // normal command with '/'
                    {
                        execv(t->argv[0],t->argv);
                        exit(EXIT_FAILURE);
                    }
                    
                  default:                  // original parent process
                    while(wait(&exitstatus) != pid);
                    
                    break;
            }
            fflush(stdout);

        // normal, exit commands
	exitstatus	= EXIT_SUCCESS;
    }

    return exitstatus;
}

void pathParser (char *string)

{
    const char *delimiter = ":";
    char *token;
	token = strtok(string, delimiter);
    int no_of_directories=0;
    
    while( token != NULL )
    {
        DIRECTORIES[no_of_directories]  = token;
               token = strtok(NULL, delimiter);
        no_of_directories++;
    }
}




