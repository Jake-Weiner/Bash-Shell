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


int execute_cmdtree(CMDTREE *t)
{
    int  exitstatus =0;
	
    if (t == NULL)
    {			// hmmmm, a that's problem
        exitstatus	= EXIT_FAILURE;
    }
    else {
        else
        {
            
            if(strncmp (t->argv[0] , "exit" , 4) == 0) //exit command
            {
                if (t->argc == 1)
                    exit(exitstatus);
                else
                {exitstatus = atoi (t->argv[1]);
                    printf("%d",exitstatus);
                    exit(exitstatus);
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
                            
                            if(strncmp (t->argv[0] , "cd" , 2) == 0) // if the command is cd then follow these conditions
                            {
                                if(t->argc == 1) // No path is specified so the directory is changed to the default HOME directory
                                {
                                    if (chdir (HOME) ==-1)  //
                                    { perror("ERROR");		// how does this work?
                                        exit(EXIT_FAILURE);
                                    }
                                    else                      // normal command with '/'
                                    {
                                        execv(t->argv[0],t->argv);
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
                                         
                                         default:                  // original parent process
                                         while(wait(&exitstatus) != pid);
                                         
                                         break;
                                        
                                    }
                                    
                                }
                                fflush(stdout);
                                int  pid;
                                
                                switch (pid = fork()) {
                                    case -1 :
                                        perror("fork() failed");     // process creation failed
                                        exit(1);
                                        break;
                                        
                                    case 0:// a new child process is created
                                        
                                        if ((strchr(t->argv[0],'/')) == NULL )  // normal command without '/'
                                        {
                                            pathParser(PATH);
                                            for (int i=0; i<CURRENT_NO_OF_DIRECTORIES ; i++)
                                            {
                                                strcat(DIRECTORIES[i],"/");
                                                strcat(DIRECTORIES[i], t->argv[0]) ;
                                                
                                                if (execv(DIRECTORIES[i], t->argv) == -1)
                                                {
                                                    continue;
                                                }
                                                
                                                break;
                                            }
                                            exit(EXIT_FAILURE);
                                        }
                                        else                      // normal command with '/'
                                        {
                                            execv(t->argv[0],t->argv);
                                            exit(EXIT_FAILURE);
                                        }
                                        
                                        exit(EXIT_FAILURE);
                                    default:					  // original parent process
                                        while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                                        break;
                                }
                                fflush(stdout);
                                
                                // normal, exit commands
                                exitstatus	= EXIT_SUCCESS;
                                // normal, exit commands
                                exitstatus	= EXIT_SUCCESS;
                            }
                            
                            return exitstatus;
                        
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
                            
                            
