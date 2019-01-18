#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main () 
{
    int child = fork();
    int status;
    int w;
    
    //  Fork first child...
    if ( child == 0 )
    {
        printf ( "I am child 1, my PID is: %i\n", getpid() );
        return 0;
    }
    //  Child 1 Error Handling:
    else if ( child < 0 )
    {
        printf ( "There was an error forking child 1. Error: %s\n",
                  strerror ( errno ) );
    }
    //  wait for child 1 to complete.
    w = waitpid ( child, &status, 0 );
    if ( w < 0 ) 
    {
        printf ( "There was a state error with child 1. Error: %s\n",
                    strerror ( errno ) );
    }

    //  Fork second child...
    child = fork();
    if ( child == 0 )
    {
        printf ( "I am child 2, my PID is %i\n", getpid() );
        return 0;
    }
    //  Child 2 Error Handling:
    else if ( child < 0 )
    {
        printf ( "There was an error forking child 2. Error: %s\n",
                    strerror ( errno ) );
    }

    //  Parent Process completion:
    w = waitpid ( child, &status, 0 );
    if ( w < 0 ) 
    {
        printf ( "There was a state error with child 2. Error: %s\n",
                    strerror ( errno ) );
    }

    printf ( "I am the parent, my PID is: %i\n", getpid() );

    return 0;
}
