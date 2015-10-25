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
int start_time_sec ;
int start_time_usec;
int stop_time_sec;
int stop_time_usec;


int  exitstatus =0;                            
int do_exit(CMDTREE *t)
{
   if (t->argc == 1)
                {
                    exit(exitstatus);
                }
                else  
                {
                    exitstatus = atoi (t->argv[1]);  // USER INPUT EXIT STATUS NUMBER
                    printf("%d",exitstatus);
                    exit(exitstatus);
                    
                }
                return exitstatus;
}

int do_cd(CMDTREE *t)
{
   if(t->argc == 1)   // NO ARUGMENTS , USE HOME
   {

    if(chdir(HOME) == -1)
      {
    perror("change directory fails");
    exit(EXIT_FAILURE);
      }
   }
   else if( t->argc ==2 && strchr(t->argv[1],'/' )== NULL) //INPUT ARGUMENT DOESN'T HAVE  '/'
   {
    //printf("the CDPATH is %s\n",getenv(CDPATH));
    char *tokencopy;
   
    char *token = strtok(CDPATH,":");
    //printf("fisrt token is %s\n",token);
    

    while(token != NULL)
    {
       tokencopy = malloc(sizeof(char) *(strlen(t->argv[1]) + strlen(token)) +1);
       strcpy(tokencopy,token);
      //printf("first token copy is %s\n",tokencopy);
       strcat(tokencopy,"/");
      //printf("after strcat1 is %s\n",tokencopy);
       strcat(tokencopy,t->argv[1]);
       //printf("after strcat2 is %s\n",tokencopy);

    if(chdir(tokencopy)!=0)
      {
      printf("-mysh: cd: %s:%s\n",tokencopy,strerror(errno)); // ERROR MESSAGE IF CHDIR FAILS
      }
      token = strtok(NULL,":");
        
    }
    free(token);
    free(tokencopy);
   
   }
 
  else if(t->argc ==2 && strchr(t->argv[1],'/' )!= NULL)  //INPUT ARGUMENT HAS  '/'
        {
            char *useful_path= NULL;
            useful_path = malloc(sizeof(char) * strlen(t->argv[1]) + 3); // INITIALIZE A SPACE FOR THE PATH TO BE PUT IN
            if (useful_path == NULL)
            {
                printf("cd: malloc failed");
            }
            strcpy(useful_path,"./");  // COPY CURRENT DIRECTORY INTO THE SPACE
            strcat(useful_path,t->argv[1]); //APPEND INPUT DIRECTORY
            if (chdir(useful_path) != 0) // CHANGE DIRECTORY, IF NOT SUCCESSFUL, PRINT ERROR
            {
                printf("-mysh: cd: %s:%s\n",useful_path,strerror(errno));
                free(useful_path);
            }
            
        }
 return exitstatus;
}

int do_time(CMDTREE *t)
{
     if ((strcmp(t->argv[0],"time") ==0) && (t->argc >1))  //  if timing is required of a command
        {
            struct timeval  start_time;
            struct timeval  stop_time;
            long double start_time_sec ;
            long double start_time_usec;
            long double stop_time_sec;
            long double stop_time_usec;
            int pid;
            switch (pid = fork())
            {
                case -1 :
                    perror("fork() failed");     // process creation failed
                    exit(1);
                    break;
                    
                case 0:// a new child process is created
                    gettimeofday(&start_time, NULL);
                    start_time_sec = (long double)start_time.tv_sec;
                    start_time_usec=(long double)start_time.tv_usec;
                    printf("program started at %Lf %Lf\n",
                           start_time_sec, start_time_usec);
                    if (execv(t->argv[0], t->argv) ==0)
                        break;
                    
                    exit(EXIT_FAILURE);
                    
                    
                default:                      // original parent process
                    while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                    gettimeofday(&stop_time, NULL );
                    stop_time_sec = (long double)stop_time.tv_sec;
                    stop_time_usec = (long double)stop_time.tv_usec;
                    printf("program stopped at  %Lf %LF usec\n",stop_time_sec,stop_time_usec);
                    /*printf("program stopped at %Lf %Lf\n",
                     stop_time_sec, stop_time_usec );
                     fprintf(stderr,"program ran for %Lf",stop_time_sec - start_time_sec + ((stop_time_usec - start_time_usec)*0.000001));
                     */
                    
                    /*gettimeofday(&stop_time, NULL );
                     
                     
                     stop_time_sec = (int)stop_time.tv_sec;
                     stop_time_usec = (int)stop_time.tv_usec;
                     
                     printf("program stopped at %i.06%i\n",
                     stop_time_sec, stop_time_usec );
                     
                     printf("%d\n",stop_time_sec);
                     printf("%d\n",start_time_sec);
                     printf ("%d\n",stop_time_sec - start_time_sec );
                     printf("program ran for %i.06%i\n",stop_time_sec - start_time_sec , stop_time_usec - start_time_usec);
                     */
                    break;
            }

        }
        return exitstatus;

}

int do_ls(CMDTREE *t)
{
  int pid;
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
                                 return exitstatus;    
}
       
int do_binls(CMDTREE *t)
{
       char *pathlist[10];
       int pid;

                         switch (pid = fork()) 
                                 {
                                    case -1 :
                                        perror("fork() failed");     // process creation failed
                                        exit(1);
                                        break;
                                        
                                    case 0:// a new child process is created
                                         // printf("%s\n",getenv("PATH"));
                                        if(PATH == NULL)
                                        {
                                          perror("Path is null");
                                          exit(EXIT_FAILURE);
                                        }
                                        

                                        char *token = strtok(PATH,":"); //SEPERATE THE PATH VARIABLE
                                        
                                        int n = 0;
                                        while(token !=NULL)
                                        {
                                          pathlist[n] = strdup(token);   
                                          token = strtok(NULL,":"); 
                                          strcat(pathlist[n],"/");   //APPEND '/' FOR THE PATH
                                          strcat(pathlist[n],t->argv[0]); // APPEND INPUT ARGUMENT FOR THE PATH
                                         // printf("%s\n",pathlist[n]);  
                                          execv(pathlist[n],t->argv); // EXECUTE THE SYSTEM CALL
                                          n++;
                                          
                                        } 
                                        exit(EXIT_FAILURE); 

                                    default:                      // original parent process
                                        while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                                        break;
                                }
                                fflush(stdout);
                                return exitstatus;

}
int execute_cmdtree(CMDTREE *t)
{
    //int  exitstatus =0;

    
    if (t == NULL)
    {           // hmmmm, a that's problem
        exitstatus  = EXIT_FAILURE;
    }
  // --------------------------------- EXIT COMMAND
    else if(strcmp (t->argv[0] , "exit")== 0) 
            {
               do_exit(t);
            }

// ----------------------------------- change direcotry
          else  if(strcmp (t->argv[0],"cd")== 0) // if the command is cd then follow these conditions
 {
  do_cd(t);
 }
             
// ---------------------------------ls,/bin/ls
            else   if ((strchr(t->argv[0],'/')) != NULL ) // INPUT ARGUMENT DOES NOT HAVE '/'
               {
                do_ls(t);
              }
               else if ((strchr(t->argv[0],'/')) == NULL && strcmp (t->argv[0],"cd")!= 0 && strcmp (t->argv[0],"time")!= 0) 
               {
                do_binls(t);
               }
               else if(strcmp (t->argv[0],"cd")== 0) // arbitrary command, print errors;
               {
                do_time(t);
               }
               
                return exitstatus;
}


