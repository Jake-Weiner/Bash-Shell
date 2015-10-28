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

int timeCommand(CMDTREE *t);
int exitCommand(CMDTREE *t);
int cdCommand(CMDTREE *t);
int specifiedInternalCommand(char **latest_argv);
int unspecifiedInternalCommand(char **latest_argv);
int do_N_COMMAND(CMDTREE *t);
int do_N_SEMICOLON (CMDTREE *t);
int do_N_AND(CMDTREE *t);
int do_N_OR(CMDTREE *t);


int do_N_SEMICOLON(CMDTREE *t)
{
  if (t == NULL)
    {           // hmmmm, a that's problem
        exitstatus  = EXIT_FAILURE;
    }
  execute_cmdtree(t->left);
  execute_cmdtree(t->right);
  return exitstatus;

}

int nBackground(CMDTREE *t)
{
    int pid;

    
    switch (pid =fork())
    {
		case -1 :
            perror("fork() failed");     // process creation failed
    		return exitstatus;
    
		case 0  :
    execute_cmdtree(t->left);
	    exit(EXIT_FAILURE);
    
	default :
       
            execute_cmdtree(t->right);
            break;
                exit(EXIT_FAILURE);
    }
    return exitstatus;
}

int nSubshell (CMDTREE *t)
{
    int pid;
    switch (pid = fork())
    {
        case -1 :
            perror("fork() failed");     // process creation failed
    		return exitstatus;
            
        case 0 :
            execute_cmdtree(t->left);
            exit(EXIT_FAILURE);
        default :
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
  break;

  case N_BACKGROUND: // as in   cmd1 &
        nBackground(t);
        break;

  case N_OR:    // as in   cmd1 || cmd2 
  break;

  case N_SEMICOLON:    // as in   cmd1 ;  cmd2 
  exitstatus = do_N_SEMICOLON(t);
  break;
        
        

  case N_PIPE:   // as in   cmd1 |  cmd2 
  break;
        

  case N_SUBSHELL:  // as in   ( cmds )
        
     exitstatus = nSubshell(t);
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
                free(useful_path);
            }
            
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
                gettimeofday(&start_time, NULL);        // gets the time that the time command is started
                start_time_sec = (long double)start_time.tv_sec;  // time in seconds
                start_time_usec=(long double)start_time.tv_usec;  // time in microseconds
                printf("program started at %Lf %Lf\n",
                       start_time_sec, start_time_usec);
                if ((strchr(t->argv[1],'/')) == NULL ) // if the command given does not have specified location
                {
                    
                   exitstatus=unspecifiedInternalCommand(tempargv);
                }
        
                else
                    exitstatus = specifiedInternalCommand(tempargv); //  if the command given does have specified location
               
               
                exit(EXIT_FAILURE);
                
                
            default:                      // original parent process
                sleep(1);
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
         return exitstatus;
   
        exit(EXIT_FAILURE);
    
}

int specifiedInternalCommand(char **latest_argv) //specified location of internal command such as ls
{
	
   int pid;
   switch (pid = fork())
          {
                                    case -1 :
                                        perror("fork() failed");     // process creation failed
                                        exit(EXIT_FAILURE);
                                        
                                        
                                    case 0:// a new child process is created
                                    execv(latest_argv[0],latest_argv);
                                    exit(EXIT_FAILURE);

                                    default:                      // original parent process
                                        while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                                        break;
            }
                                fflush(stdout);
                                 return exitstatus;    
}
       
int unspecifiedInternalCommand(char **latest_argv) //unspecified location of internal command such as ls
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
                                          strcat(pathlist[n],latest_argv[0]); // APPEND INPUT ARGUMENT FOR THE PATH
                                          execv(pathlist[n],latest_argv); // EXECUTE THE SYSTEM CALL
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

int do_N_COMMAND(CMDTREE *t) // EXECUTION IF TYPE IS N_COMMAND
{
    int fd;

       if (t->infile != NULL )  // the infile is used as the input instead of stdin
       {
                fd = open(t-> infile, O_RDONLY);
                dup2(fd, 0);
                close (fd);
       }
    
        if ((t->outfile != NULL) && (t->append == false ))  // the outfile is used as the output instead of stdout
        {
            fd = open(t-> outfile, O_WRONLY | O_TRUNC | O_CLOEXEC |O_CREAT, S_IWUSR |S_IRUSR | S_IXUSR);
            printf("open return value is %d\n" , fd);
            dup2(fd,1);
            close(fd);
        }
    
    	if ((t->outfile != NULL) && (t->append == true))
        {
            fd = open(t-> outfile, O_WRONLY |O_APPEND|O_CREAT, S_IWUSR |S_IRUSR | S_IXUSR);
            printf("open return value is %d\n" , fd);
            dup2(fd,1);
            close(fd);
        }
            
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
                exitstatus = specifiedInternalCommand(t->argv);
            }
            else if ((strchr(t->argv[0],'/')) == NULL && strcmp (t->argv[0],"cd")!= 0 && strcmp (t->argv[0],"time")!= 0)
            {
                exitstatus =unspecifiedInternalCommand(t->argv);
            }
            else if(strcmp (t->argv[0],"time")== 0) // time command
            {
            
                exitstatus = timeCommand(t);
            }
        

            return exitstatus;
    exit(EXIT_FAILURE);
    
}

