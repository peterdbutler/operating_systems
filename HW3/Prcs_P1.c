#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main ( int argc, char* argv[] )
{
    
    char dest1[] = { "destination1.txt" };
    char dest2[] = { "destination2.txt" };
    int fd_1, fd_2;

    fd_1 = open ( dest1, O_CREAT | O_EXCL, S_IRWXU );
    if ( fd_1 < 0 )
    {
        printf ( "'destination1.txt' creation error: %s\n", strerror ( errno ) );
        return 0;
    }

    fd_2 = open ( dest2, O_CREAT | O_EXCL, S_IRWXU );
    if ( fd_2 < 0 )
    {
        printf ( "'destination2.txt' creation error: %s\n", strerror ( errno ) );
        return 1;
    }

    return 0;
}
