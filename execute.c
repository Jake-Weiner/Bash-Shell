#include "mysh.h"

/*
 CITS2002 Project 2 2015
 Name(s):		student-name1 (, student-name2)
 Student number(s):	student-number-1 (, student-number-2)
 Date:		date-of-submission
 */
// cc -std=c99 -Wall -Werror -pedantic execute.c globals.c mysh.c parser.c
// -------------------------------------------------------------------

//  THIS FUNCTION SHOULD TRAVERSE THE COMMAND-TREE and EXECUTE THE COMMANDS
//  THAT IT HOLDS, RETURNING THE APPROPRIATE EXIT-STATUS.
//  READ print_cmdtree0() IN globals.c TO SEE HOW TO TRAVERSE THE COMMAND-TREE


//  THAT IT HOLDS, RETURNING THE APPROPRIATE EXIT-STATUS.
//  READ print_cmdtree0() IN globals.c TO SEE HOW TO TRAVERSE THE COMMAND-TREE



void pathParser (char *string)

{
    char *copy;
    copy = strdup(string);
    const char *delimiter = ":";
    char *token;
    token = strtok(string, delimiter);
    token = strtok(copy, delimiter);
    CURRENT_NO_OF_DIRECTORIES=0;

    DIRECTORIES = (char**)malloc(sizeof(char**) * 1);
    while( token != NULL )
    {
        DIRECTORIES[CURRENT_NO_OF_DIRECTORIES]  = token;
        token = strtok(NULL, delimiter);
        DIRECTORIES = (char**)realloc(DIRECTORIES, sizeof(char**) * (CURRENT_NO_OF_DIRECTORIES + 1));
        DIRECTORIES[CURRENT_NO_OF_DIRECTORIES] = malloc(sizeof(char) * strlen((token + 1)));
        strcpy(DIRECTORIES[CURRENT_NO_OF_DIRECTORIES], token);
        token = strtok(NULL, delimiter);
        CURRENT_NO_OF_DIRECTORIES++;
    }
}

int execute_cmdtree(CMDTREE *t)
{
    int  exitstatus =0;
    
    if (t == NULL)
    {			// hmmmm, a that's problem
        exitstatus	= EXIT_FAILURE;
    }
    
    // ---------------------exit execution
    
    if(strncmp (t->argv[0] , "exit" , 4) == 0) //exit command
    {
        if (t->argc == 1) //standard exit
            exit(0);
        if(t->argc == 2)   // argument provided
        {
            exitstatus = atoi (t->argv[1]); // set exitstatus to what the user wants
            printf("%d",exitstatus);
            exit(exitstatus);
        }
        
        
    }
    
    // --------------- cd execution
    
    
    if(strncmp (t->argv[0] , "cd" , 2) == 0) // if the command is cd then follow these conditions
    {
        if(t->argc == 1) // No path is specified so the directory is changed to the default HOME directory
        {
            chdir (HOME);
            
            if (chdir (HOME) ==-1)  // print error if chdir fails
            {
                perror("ERROR");
                exit(EXIT_FAILURE);
            }
            
            else if (t->argc == 2) //  if an arguement is passed to the cd command
            {
                char *current_working_directory = (char*)malloc(sizeof(char) * 2);
                strcat(current_working_directory,"./");
                current_working_directory = (char *)realloc(current_working_directory,sizeof(current_working_directory) + (sizeof(char) * (strlen(t->argv[1]))));
                strcat(current_working_directory,t->argv[1]);
                
                if( chdir(current_working_directory) ==-1)
                    { 	perror("ERROR");
                        exit(EXIT_FAILURE);
                    }
            }
        }
        exit(EXIT_SUCCESS);
    }
    // ------------normal command execution
    
    int pid;
    switch (pid = fork())
    {
        case -1 :
            perror("fork() failed");     // process creation failed
            exit(EXIT_FAILURE);
            break;
            
        case 0:// a new child process is created
            
            if ((strchr(t->argv[0],'/')) == NULL )  // normal command without '/'
                {
                    pathParser(PATH);  //parses PATH into a pointer array called DIRECTORIES
                    for (int i=0; i<CURRENT_NO_OF_DIRECTORIES ; i++)
                    {
                        strcat(DIRECTORIES[i],"/");              // appending a back slash for every directory
                        strcat(DIRECTORIES[i], t->argv[0]) ;        // appending the directory with user command to form a complete directory
                        if (execv(DIRECTORIES[i], t->argv) == -1)    // keep trying different directories until successful
                        {
                            continue;
                        }
                        break;          // stop trying once successful
                        exit(EXIT_FAILURE);
                    }
                }
            else                      // normal command with '/'
                {
                    execv(t->argv[0],t->argv);
                    exit(EXIT_FAILURE);
                }
            break;
        default:					  // original parent process
            while(wait(&exitstatus) != pid); // waits for the child process to finish running;
            break;
            
            
            // normal, exit commands
    
      }
    
    
    fflush(stdout);

    return exitstatus;
    
}

