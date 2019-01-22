#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main ( int argc, char* argv[] )
{
    int status;
    int child_1 = fork();

    if ( child_1 < 0 )
    {
        printf ( "There was an error forking child 1: %s\n", strerror ( errno ) );
        return 0;
    }

    if ( child_1 == 0 )
    {
        execv ( "Prcs_P1", &argv[0] );

        //  error handling (will only print if above execvp does not execute:
        printf ( "An error executing 'Prcs_P1' has occured.\n%s%s",
                 "USAGE NOTE: Please compile Prcs_P1.c & Prcs_P2.c as ",
                 "'Prcs_P1' & 'Prcs_P2' respectively\n" );
        return 0;
    }

    //  Parent waits for child_1 to complete:
    waitpid ( child_1, &status, 0 );

    int child_2 = fork();
    if ( child_2 < 0 )
    {
        printf ( "There was an error forking child 2: %s\n", strerror ( errno ) );
        return 0;
    }

    if ( child_2 == 0 )
    {
        execvp ( "./Prcs_P2", &argv[0] );

        //  error handling (will only print if above execvp does not execute:
        printf ( "An error executing 'Prcs_P2' has occured.\n%s%s",
                 "USAGE NOTE: Please compile Prcs_P1.c & Prcs_P2.c as ",
                 "'Prcs_P1' & 'Prcs_P2' respectively\n" );
        return 0;
    }

    //  Parent waits for child_2 to complete:
    waitpid ( child_2, &status, 0 );

    return 0;
}
