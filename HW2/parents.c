#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main () 
{
    //  parent p
    int a = 10;
    int b = 25;
    int fq = 0;
    int fr = 0;
    
    fq = fork();    //  fork a child -  call it Process Q
    if ( fq == 0 )
    {
        a = a+b;
        printf ( "PID: %i\na = %i\nb = %i\n", getpid(), a, b );
        
        fr = fork();    // fork another child -  call it Process R
        if ( fr != 0 )
        {
            b = b+20;
            printf ( "PID: %i\na = %i\nb = %i\n", getpid(), a, b );
        }
        else
        {
            a = ( a*b )+30;
            printf ( "PID: %i\na = %i\nb = %i\n", getpid(), a, b );
        }
    }
    else 
    {
        b = a+b-5;
        printf ( "PID: %i\na = %i\nb = %i\n", getpid(), a, b );
    }
    return 0;
}
