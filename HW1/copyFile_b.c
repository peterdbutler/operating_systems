#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main ( int argc, char* argv[] )
{
    //  path strings
    char* in_path = argv [1];
    char* out_path;
    
    //  access check var
    int returnval;

    //  file descriptors
    int in_fd, out_fd;

    //  read, write counters
    ssize_t byteread = 1;
    ssize_t bytewrite;

    //  buffer vars
    size_t count = 100;
    char fbuff[count];

    //  for appending "XYZ" after each write
    char xyz[3] = { 'X','Y','Z' };

    //  check usage:
    if ( argc != 3 )
    {
        if ( argc < 3 )
            printf ( "Error: too few arguments supplied\n" );
        else 
            printf ( "Error: too many arguments supplied\n" );

        //  print usage
        char* prgm_name = argv[0];
        printf ( "Usage: \t[%s] soureFile destinationFile\n\t\t--sourceFile must exist and be readable by user\n", prgm_name );
        return 0;
    }

    //  in_path access:
    returnval = access ( in_path, F_OK );
    if ( returnval != 0 )
    {
        printf ( "Source File Error: %s\n", strerror ( errno ) );
        return 0;
    }

    //  open input file:
    in_fd = open ( in_path, O_RDONLY );
    if ( in_fd < 0 )
    {
        printf ( "Source file open error: %s\n", strerror ( errno ) );
    }

    //  check out_path access && open (or create)
    out_path = argv[2];
    out_fd = open ( out_path, O_WRONLY | O_CREAT, 0666 );

    // check success:
    if ( out_path < 0 )
    {
        printf ( "Destination file open error: %s\n", strerror ( errno ) );
        return 1;
    }

    //  copy file (read in_fd, write out_fd)
    while ( byteread > 0 )
    {
        //  read source file
        byteread = read ( in_fd, fbuff, count );
        if ( byteread < 0 ) 
        {
            printf ( "read error: %s\n", strerror ( errno ) );
            return 1;
        }
        
        //  replace '1' w/ 'A'
        for ( int i = 0; i < count; i++ )
        {
            if ( fbuff [i] == '1' )
                fbuff [i] = 'A';
        }

        //  write to destination file
        if ( byteread > 0 )
            bytewrite = write ( out_fd, fbuff, byteread );
        
        if ( byteread > 0 && byteread != bytewrite )
        {
            printf ( "A read/write error has occured. Copy failed and aborted\n" );
            return 1;
        }

        //  Append XYZ after each write
        if ( byteread > 0 )
        {
            bytewrite = write ( out_fd, xyz, 3 );
            if ( bytewrite != 3 )
            {
                printf ( "A read/write error has occured. Copy failed and aborted\n" );
                return 1;
            }
        }
    }

    //  close files and end program
    returnval = close ( out_fd );
    if ( returnval < 0 )
    {
        printf ( "Failed to close [%s] copy Error: %s\n",
                 in_path, strerror ( errno ) );
        return 1;
    }
    
    returnval = close ( in_fd );
    if ( returnval < 0 )
    {
        printf ( "Failed to close [%s] Error: %s\n", in_path, strerror ( errno ) );
        return 1;
    }
    return 0;
}
