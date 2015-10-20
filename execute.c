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
    printf("%s",PATH);
    int  exitstatus;
    
    if(t == NULL)
    {			// hmmmm, a that's problem
        exitstatus = EXIT_FAILURE;
        exit(exitstatus);

    }
 
    if((strncmp (t->argv[0] , "exit" , 4) == 0)&& t->argc == 1)//exit command
        {
            exitstatus = EXIT_SUCCESS;
            exit(exitstatus);
        }
    
    if((strncmp (t->argv[0] , "exit" , 4) == 0)&& t->argc != 1)
    {
        exitstatus = atoi(t->argv[1]);
      //  printf(" %d",t->argc);
      //  printf("exit  %d",atoi(t->argv[1]));
        exit(exitstatus);
    }
        
        if(strncmp (t->argv[0] , "cd" , 2) == 0) // attempt to change directory
        {
            if(t->argc == 1) // default path HOME
            {
                chdir ("bin/ls");
                perror("change directory1 fails");
                exit(EXIT_FAILURE);
            }
            
            else  if(t->argc == 2 && (strchr(t->argv[1],'/')) == NULL ) // Given a path without '/'
            {
                chdir (t->argv[1]);
                perror("change directory2 fails");
                exit(EXIT_FAILURE);
            }
            else if(t->argc == 2 && (strchr(t->argv[1],'/')) != NULL )            // Given a full path
            {
                chdir (t->argv[1]);
                perror("change directory3 fails");
                exit(EXIT_FAILURE);
            }
            
        }

        
        int  pid;
        
        switch (pid = fork()) {
            case -1 :
                perror("fork() failed");     // process creation failed
                exit(1);
                break;
                
            case 0:// new child process
                
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
        
    
    
    return exitstatus;
}
