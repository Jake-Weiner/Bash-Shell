#include "mysh.h"

/*
 CITS2002 Project 2 2015
 Name(s):       student-name1 (, student-name2)
 Student number(s): student-number-1 (, student-number-2)
 Date:      date-of-submission
 */
// cc -std=c99 -Wall -Werror -pedantic execute.c globals.c mysh.c parser.c 
// -------------------------------------------------------------------

//  THIS FUNCTION SHOULD TRAVERSE THE COMMAND-TREE and EXECUTE THE COMMANDS
//  THAT IT HOLDS, RETURNING THE APPROPRIATE EXIT-STATUS.
//  READ print_cmdtree0() IN globals.c TO SEE HOW TO TRAVERSE THE COMMAND-TREE


//  THAT IT HOLDS, RETURNING THE APPROPRIATE EXIT-STATUS.
//  READ print_cmdtree0() IN globals.c TO SEE HOW TO TRAVERSE THE COMMAND-TREE
// -------------------change directory
                                



int execute_cmdtree(CMDTREE *t)
{
    int  exitstatus =0;
    char *pathlist[10];
    
    if (t == NULL)
    {           // hmmmm, a that's problem
        exitstatus  = EXIT_FAILURE;
    }
  // --------------------------------- exit command 
    if(strcmp (t->argv[0] , "exit")== 0) 
               {
                if (t->argc == 1)
                {
                    exit(exitstatus);
                }
                else
                {
                    exitstatus = atoi (t->argv[1]);
                    printf("%d",exitstatus);
                    exit(exitstatus);
                    
                }
            }

// ----------------------------------- change direcotry
            if(strcmp (t->argv[0],"cd")== 0) // if the command is cd then follow these conditions
 {
   if(t->argc == 1)
   {
    printf("dddddddddddd %s\n",getenv(HOME));
    if(chdir(HOME) == -1)
      {
    perror("change directory fails");
    exit(EXIT_FAILURE);
      }
   }
   else if( t->argc ==2 && strchr(t->argv[1],'/' )== NULL)
   {
   }
 } 
             
// ---------------------------------ls,/bin/ls
             int  pid;


               if ((strchr(t->argv[0],'/')) != NULL )
               {
                         switch (pid = fork()) 
                                 {
                                    case -1 :
                                        perror("fork() failed");     // process creation failed
                                        exit(1);
                                        break;
                                        
                                    case 0:// a new child process is created
                                    execv(t->argv[0], t->argv);
                                    exit(EXIT_FAILURE);
                                        
                                    default:                      // original parent process
                                        while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                                        break;
                                }
                                fflush(stdout);
                 }
                                

               if ((strchr(t->argv[0],'/')) == NULL )
               {
                


                         switch (pid = fork()) 
                                 {
                                    case -1 :
                                        perror("fork() failed");     // process creation failed
                                        exit(1);
                                        break;
                                        
                                    case 0:// a new child process is created
                                        printf("%s\n",getenv("PATH"));
                                        if(PATH == NULL)
                                        {
                                          perror("PAth is null");
                                          exit(EXIT_FAILURE);
                                        }
                                        

                                        char *token = strtok(PATH,":");
                                        
                                        int n = 0;
                                        while(token !=NULL)
                                        {
                                          pathlist[n] = strdup(token);
                                          token = strtok(NULL,":"); 
                                          strcat(pathlist[n],"/");
                                          strcat(pathlist[n],t->argv[0]);
                                          printf("%s\n",pathlist[n]);
                                          execv(pathlist[n],t->argv);
                                          n++;
                                          
                                        } 
                                        exit(EXIT_FAILURE); 

                                    default:                      // original parent process
                                        while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                                        break;
                                }
                                fflush(stdout);
                            }
                                

                            
                             fflush(stdout);
                return exitstatus;
}


/*
printf("current number %d",CURRENT_NO_OF_DIRECTORIES);
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
*/
