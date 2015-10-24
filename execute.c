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
    char *pathlist[7];
    
    if (t == NULL)
    {           // hmmmm, a that's problem
        exitstatus  = EXIT_FAILURE;
    }
   
    if(strcmp (t->argv[0] , "exit")== 0) //exit command
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
                                        

                                        char *token = strtok(PATH,":");
                                        printf("token 111 %s\n",token);
                                        int n = 0;
                                        while(token !=NULL)
                                        {
                                          pathlist[n] = strdup(token);
                                          token = strtok(NULL,":"); 
                                          strcat(pathlist[n],"/");
                                          strcat(pathlist[n],t->argv[0]); 
                                          n++;
                                          
                                        } 
                                        for(int j=0;j<6;j++)
                                        {
                                          printf("%s\n",pathlist[j]);
                                          execv(pathlist[j],t->argv);
                                        }  
                                        exit(EXIT_FAILURE); 

                                    default:                      // original parent process
                                        while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                                        break;
                                }
                                fflush(stdout);
                            }
                                


// -------------------change directory
                                if(strncmp (t->argv[0] , "cd" , 2) == 0) // if the command is cd then follow these conditions
                        {
                switch (pid = fork()) 
                     {
                    case -1 :
                        perror("fork() failed");     // process creation failed
                        exit(1);
                        break;
                        
                    case 0:// new child process
                        
                            if(t->argc == 1) // No path is specified so the directory is changed to the default path HOME
                            {
                                chdir (HOME);
                                perror("change directory fails");
                                exit(EXIT_FAILURE);
                            }
                        
                        
                            
                                      if (t->argc == 2) //  if an arguement is passed to the cd command
                                    {
                                        char *current_working_directory = (char*)malloc(sizeof(char) * 2);
                                        strcat(current_working_directory,"./");
                                        current_working_directory = (char *)realloc(current_working_directory,sizeof(current_working_directory) + (sizeof(char) * (strlen(t->argv[1]))));
                                        strcat(current_working_directory,t->argv[1]);
                                        
                                        
                                        if( chdir(current_working_directory) ==-1)
                                         {  perror("ERROR");
                                         exit(EXIT_FAILURE);
                                         }
                                         
                                         default:                  // original parent process
                                         while(wait(&exitstatus) != pid);
                                         
                                         break;
                                        
                                    }

                              }
                               
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
