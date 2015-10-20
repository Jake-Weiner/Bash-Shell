//
//  path-parser.c
//  
//
//  Created by Jake Weiner on 16/10/2015.
//
//

#include <string.h>
#include <stdio.h>

int path_parse ()
{
    
    char str[32] = "/bin:/usr/bin:/usr/local/bin:.";
    const char s[2] = ":";
    char *token;
    char *storage[5];
    
    
    
    token = strtok(str, s);
   
   
    printf("first token \n");
    
    int i = 0;
    
    while( token != NULL )
    {
        storage[i]  = token;
        printf("%s\n",storage[i]);
        token = strtok(NULL, s);
        printf("%s\n",storage[i]);
        
        i++;
    }
    
  /*  for(int j = 0;j<5;j++)
    {
        printf("%s\n",storage[j]);
        
        //strcat (storage[j], "/");
  */  }

    
    return(0);
    
    
}




int main(void)
{
    path_parse();
}
