#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main ( int argc, char* argv[] )
{
    char* filepath = argv[1];
    int returnval;
    int fd;
    size_t count = 100;
    char fbuff[count];
    ssize_t byteread;

    //  check argc, valid at ONLY 2!
    if ( argc != 2 )
    {
        printf ( "invalid arguments\n" );
        return 0;
    }

    //  check file existance
    returnval = access ( filepath, F_OK );
    if ( returnval != 0 )
    {
        if ( errno == ENOENT )
            printf ( "%s does not exist\n", filepath );
        else if ( errno == EACCES )
            printf ( "%s is not accessible \n", filepath );
        else
            printf ( "Error: %s\n", strerror ( errno ) );
        return 0;
    }

    //  check read access:
    returnval = access ( filepath, R_OK );
    if ( returnval < 0 )
    {
        printf ( "Read error: %s\n", strerror ( errno ) );
        return 0;
    }

    //  open file
    errno = 0;
    fd = open ( filepath, O_RDONLY );

    if ( fd < -1 )
    {
        printf ( "failed to open %s\nError: %s\n", filepath, strerror ( errno ) );
        return 1;
    }

    //  display file
    printf ( "\n" );
    while ( byteread > 0 )
    {
        byteread = read ( fd, fbuff, count );
        if ( byteread < 0 ) 
        {
            printf ( "read error: %s\n", strerror ( errno ) );
            return 1;
        }

        //  display file
        if ( byteread > 0 )
        {
            for ( int i = 0; i < byteread ; i++ ) 
                printf ( "%c", fbuff[i] );
        }
    }

    //  close file
    errno = 0;
    returnval = close ( fd );

    if ( returnval < 0 )
    {
        printf ( "%s failed to close\nError: %s\n", filepath, strerror ( errno ) );
        return 1;
    }
    if ( returnval > 0 )
        return 1;
    return 0;
}
