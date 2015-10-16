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

#DEFINE MAX_PATHS 10

int execute_cmdtree(CMDTREE *t)
{
    int  exitstatus;

    if(t == NULL) {			// hmmmm, a that's problem
	exitstatus	= EXIT_FAILURE;
    }
    else {
        
        if(strncmp (t->argv[0] , "exit" , 4) == 0) //exit command
        {
            exit(0);
        }
       
            int  pid;
            
            switch (pid = fork()) {
                case -1 :
                    perror("fork()");     // process creation failed
                    exit(1);
                    break;
                    
                case 0:// new child process

                    if ((strchr(t->argv[0],'/')) == NULL )
                    {
                        char **parsed_paths;
                        int index;
                        int counterl
                        
                        for (int j=0 ; j<MAX_PATHS; j++)
                        {
                            while (PATH[index]!=':')
                            {
                                index++;
                            }
                         
                       parsed_paths[counter] = strndup(PATH,index)
                               counter++;
                        }
                        
                        
                        
                        printf("%s",DEFAULT_PATH);
                      // strcat(DEFAULT_PATH,"/ls");
                        //strcat(DEFAULT_PATH,t->argv[0]);
                       printf("%s",PATH);
                        execv(DEFAULT_PATH,t->argv);
                        exit(EXIT_FAILURE);
                    }
                      /*  else
                    	{printf("this is null");
                            execv(t->argv[0],t->argv);
                    			exit(EXIT_FAILURE);
                        }
                       */
                    break;
                    
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
