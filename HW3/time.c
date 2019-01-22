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

    if ( c == 0 )
    {
        printf ( "PID: %i\n", getpid() );
        execl ( "/bin/date", " ", NULL );

        //  printed in case of error:
        printf ( "execl error: %s\n", strerror ( errno ) );
    }
        
    //  guarantee child finish:
    waitpid ( c, &status, 0 );
    return 0;
}
