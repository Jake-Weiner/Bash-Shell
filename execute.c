

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



// -------------------change directory



int execute_cmdtree(CMDTREE *t)
{
    int  exitstatus =0;
    char *pathlist[10];
    
    if (t == NULL)
    {           // hmmmm, a that's problem
        exitstatus  = EXIT_FAILURE;
    }
    // --------------------------------- EXIT COMMAND
    if(strcmp (t->argv[0] , "exit")== 0)
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
    }
    
    // ----------------------------------- change directory
    if(strcmp (t->argv[0],"cd")== 0) // if the command is cd then follow these conditions
    {
        
        if(t->argc == 1)   // NO ARUGMENTS , cd to HOME directory
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
            //printf("first token is %s\n",token);
            
            
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
                    printf("-wshell: cd: %s:%s\n",tokencopy,strerror(errno)); // ERROR MESSAGE IF CHDIR FAILS
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
                printf("-wshell: cd: %s:%s\n",useful_path,strerror(errno));
                free(useful_path);
            }
            
        }
    }
    
    // ---------------------------------other commands which exit on execution such as ls, cal etc...
    int  pid;	//process id
    
    
    if ((strchr(t->argv[0],'/')) != NULL ) // input argument specifies where the command is located and contains '/'
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
    
    
    if ((strchr(t->argv[0],'/')) == NULL && strcmp (t->argv[0],"cd")!= 0) // input argument does not specifiy where the command is located need to check the directories within the PATH variable
    {
        
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
                    execv(pathlist[n],t->argv);  // EXECUTE THE SYSTEM CALL
                    n++;
                    
                }
                exit(EXIT_FAILURE);
                
            default:                      // original parent process
                while(wait(&exitstatus) != pid); // waits for the child process to finish running;
                break;
        }
        fflush(stdout);
    }
    return exitstatus;
}



