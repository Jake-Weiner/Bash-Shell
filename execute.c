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

int  exitstatus = 0;

int timeCommand(CMDTREE *t);
int exitCommand(CMDTREE *t);
int cdCommand(CMDTREE *t);
int specifiedInternalCommand(CMDTREE *t);    
int unspecifiedInternalCommand(CMDTREE *t);
int do_N_COMMAND(CMDTREE *t);
int do_N_SEMICOLON (CMDTREE *t);
int do_N_AND(CMDTREE *t);
int do_N_OR(CMDTREE *t);
int do_N_PIPE(CMDTREE *t);


int do_N_SEMICOLON(CMDTREE *t)
{
  if (t == NULL)
    {           // hmmmm, a that's problem
        exitstatus  = EXIT_FAILURE;
    }
  exitstatus = execute_cmdtree(t->left);  //DO THE COMMAND FROM LEFT TO RIGHT RESPECTIVELY
  exitstatus = execute_cmdtree(t->right);
  return exitstatus;

}

int do_N_AND(CMDTREE *t)
{
  if(t ==NULL)
  {
    exitstatus = EXIT_FAILURE;
  }
  else if(execute_cmdtree(t->left) == 0)  // IF LEFT IS SUCCESSFUL THEN DO RIGHT
  {
    exitstatus = execute_cmdtree(t->right);
  }
  else 
    exitstatus = EXIT_FAILURE;  //OTHERWISE EXIT FAILURE
  return exitstatus;
  }

int do_N_OR(CMDTREE *t)
{

 if(t ==NULL)
  {
    exitstatus = EXIT_FAILURE;
  }
  else if(execute_cmdtree(t->left) != 0)  //IF LEFT FAILS THEN DO RIGHT
  {
    exitstatus = execute_cmdtree(t->right);
  }
  else
    exitstatus = EXIT_SUCCESS;  //OTHERWISE EXIT SUCCESS
  return exitstatus;

}

int do_N_BACKGROUND(CMDTREE *t)
{
    int pid;
  
    
    switch (pid =fork())
    {
    case -1 :
            perror("fork() failed");     // process creation failed
        return exitstatus;
    
    case 0  :
    exitstatus = execute_cmdtree(t->left);
    exit(exitstatus); 
    
      return exitstatus;
    
  default :
       
            exitstatus = execute_cmdtree(t->right);
            
            break;
    }
    return exitstatus;
}

int do_N_SUBSHELL (CMDTREE *t)
{
    int pid;
    switch (pid = fork())
    {
        case -1 :
            perror("fork() failed");     // process creation failed
        return exitstatus;
            
        case 0 :
            exitstatus = execute_cmdtree(t->left);
            exit(exitstatus);
        default :
            break;
            exit(EXIT_FAILURE);
            
    }

    return exitstatus;
}

int do_N_PIPE(CMDTREE *t)
{

 int pid;   //FIRST CMD
 int pid2; // SECOND CMD
 int pipefd[2]; //ONE PIPE


    switch (pid = fork())
    {
        case -1 :
            perror("fork() failed");     // PROCESS CREATION FAILS
        return exitstatus;
            
        case 0 :     // CHILD PROCESS
         if (pipe(pipefd) == -1)
        {
        perror("pipe failed\n");
        exit(EXIT_FAILURE);
        }

        switch(pid2 = fork())
        {
          case -1 :
          perror("fork() failed");
          return exitstatus;

          case 0:
          close(pipefd[1]);
          //close(STDIN_FILENO);
          dup2(pipefd[0],STDIN_FILENO);
          close(pipefd[0]);
          exitstatus = execute_cmdtree(t->right);
          exit(EXIT_FAILURE);

          default :
        close(pipefd[0]);
        dup2(pipefd[1],STDOUT_FILENO);
         // out end of the pipe is connnected to the stdout of cmd1 
        exitstatus = execute_cmdtree(t->left);
        exit(EXIT_SUCCESS);
          //while(wait(&exitstatus) != pid2); 
          break;
        }
         
        default :
        
         while(wait(&exitstatus) != pid); // waits for the child process to finish running;   
        break;
        exit(EXIT_FAILURE);           
    }
   
      
  return exitstatus;
}

int execute_cmdtree (CMDTREE *t)
{
if (t == NULL)
    {           // hmmmm, a that's problem
        exitstatus  = EXIT_FAILURE;
    }
  switch (t->type)  //EXECUTE COMMAND BASED ON DIFFERENT TYPE 
{
  case N_AND :   // as in   cmd1 && cmd2
  exitstatus = do_N_AND(t);
  
  break;

  case N_BACKGROUND:   // as in   cmd1 &
  exitstatus = do_N_BACKGROUND(t);
  
  break;

  case N_OR:    // as in   cmd1 || cmd2
  exitstatus = do_N_OR(t); 
  
  break;

  case N_SEMICOLON:    // as in   cmd1 ;  cmd2 
  exitstatus = do_N_SEMICOLON(t);
  break;

  case N_PIPE:   // as in   cmd1 |  cmd2 
  exitstatus = do_N_PIPE (t);
  break;

  case N_SUBSHELL:  // as in   ( cmds )
  exitstatus = do_N_SUBSHELL(t);
  break;

  case N_COMMAND:
  exitstatus = do_N_COMMAND(t);
  
  break;

  default :
  printf("invalid input\n");

}


    return exitstatus;
}


int exitCommand(CMDTREE *t)
{
   if (t->argc == 1)
                {
                    exit(0);
                }
                else  
                {
                    exitstatus = atoi (t->argv[1]);  // USER INPUT EXIT STATUS NUMBER
                    printf("%d\n",exitstatus);
                    exit(exitstatus);
 
                }
                return exitstatus;
}

int cdCommand(CMDTREE *t)
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
                
            }
            free(useful_path);
        }
 return exitstatus;
}

int timeCommand(CMDTREE *t)
{
    char **tempargv;   // a temporary argv is created and is passed to as the arguments to internal command functions
  tempargv = (char**)malloc(sizeof(char**));
    for (int i=0; i<t->argc ; i++)
    {
        tempargv[i] = t->argv[i+1];
        tempargv = (char**)realloc(tempargv, sizeof(char*));
        printf ( "tempargv[%d] is now %s\n", i, tempargv[i]);
    }
        struct timeval  start_time;
        struct timeval  stop_time;
        int start_time_sec ;
        int start_time_usec;
        int stop_time_sec;
        int stop_time_usec;
        
        int pid;
        switch (pid = fork())
        {
            case -1 :
                perror("fork() failed");     // process creation failed
                exit(1);
                break;
                
            case 0:// a new child process is created
                gettimeofday(&start_time, NULL);        // gets the time that the time command is started
                start_time_sec = start_time.tv_sec;  // time in seconds
                start_time_usec= start_time.tv_usec;  // time in microseconds
                printf("program started at %d %d\n",start_time_sec, start_time_usec);
                if ((strchr(t->argv[1],'/')) == NULL ) // if the command given does not have specified location
                {
                    
                   exitstatus = unspecifiedInternalCommand(t);
                }
        
                else
                {
                    exitstatus = specifiedInternalCommand(t); //  if the command given does have specified location
                }  
               gettimeofday(&stop_time, NULL);
                stop_time_sec = stop_time.tv_sec;
                stop_time_usec = stop_time.tv_usec;
                printf("program used  %d ms\n" ,(stop_time_sec - start_time_sec) *1000 + (stop_time_usec -start_time_usec)/1000);
                exit(EXIT_FAILURE);           
            default:                      // original parent process
                
                while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                break;
        }
         return exitstatus;
   
        exit(EXIT_FAILURE);
    
}
         
int specifiedInternalCommand(CMDTREE *t) //specified location of internal command such as ls
{
  
   int pid;
   switch (pid = fork())
          {
                                    case -1 :
                                        perror("fork() failed");     // process creation failed
                                        exit(EXIT_FAILURE);
                                        
                                        
                                    case 0:// a new child process is created
                                    execv(t->argv[0],t->argv);
                                    exit(EXIT_FAILURE);

                                    default:                      // original parent process
                                        while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                                        break;
            }
                                fflush(stdout);
                                 return exitstatus;    
}
       
int unspecifiedInternalCommand(CMDTREE *t) //unspecified location of internal command such as ls
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

int setargument(CMDTREE *t)
{
  if(strcmp(t->argv[1],"PATH") == 0)
    {
      realloc(PATH,sizeof(char) *strlen(t->argv[2]) +1);
      strcpy(PATH,t->argv[2]);
      printf("PATH after set %s\n",PATH);
    }
            
  else if(strcmp(t->argv[1],"CDPATH" )== 0)
    {
      realloc(CDPATH,sizeof(char) *strlen(t->argv[2]) +1);
      strcpy(CDPATH,t->argv[2]);
      printf("CDPATH after set %s\n",CDPATH);
    }
  else if(strcmp(t->argv[1],"HOME" )==0)
    {
      realloc(HOME,sizeof(char) *strlen(t->argv[2]) +1);
      strcpy(HOME,t->argv[2]);
      printf("HOME after set %s\n",HOME);
    }
   else
    {
      fprintf(stderr, "invaild input");
      exitstatus = EXIT_FAILURE;
    }
                return exitstatus;
}


int do_N_COMMAND(CMDTREE *t) // EXECUTION IF TYPE IS N_COMMAND
{

            if(strcmp (t->argv[0] , "exit")== 0)
            {
                exitstatus = exitCommand(t);
            }
            // ----------------------------------- change directory
            else  if(strcmp (t->argv[0],"cd")== 0) // if the command is cd then follow these conditions
            {
                exitstatus =cdCommand(t);
            }
            
            // ---------------------------------ls type commands
            else   if ((strchr(t->argv[0],'/')) != NULL ) // INPUT ARGUMENT HAS '/'
            {
                exitstatus = specifiedInternalCommand(t);
            }
            else if ((strchr(t->argv[0],'/')) == NULL && strcmp (t->argv[0],"cd")!= 0 && strcmp (t->argv[0],"time")!= 0 && strcmp(t->argv[0],"set")!= 0)
            {

                exitstatus =unspecifiedInternalCommand(t);
                if((exitstatus = unspecifiedInternalCommand(t)) != 0)
                {
                  fprintf(stderr, "-mysh: %s:command not found\n", t->argv[0]);
                }
            }
            else if(strcmp (t->argv[0],"time")== 0) // time command
            {
            
                exitstatus = timeCommand(t);
            }

            else if (strcmp(t->argv[0] , "set" ) == 0 && t->argc == 3) //SET ARGUMENTS

              {

               exitstatus = setargument(t);
              }

          
              return exitstatus;
              exit(EXIT_FAILURE);
}

