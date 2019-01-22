#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main ( int argc, char* argv[] )
{
    int c = fork();
    int status;

    char *const params[] = { "ls", "-al", NULL };

    if ( c == 0 )
    {
        printf ( "PID: %i\n", getpid() );
        execvp ( "ls", &params[0] );

        //  Error message detailing failure:
        printf ( "execl error: %s\n", strerror ( errno ) );
    }
        
    //  guarantee child finish:
    waitpid ( c, &status, 0 );
    return 0;
}
