#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main ( int argc, char* argv[] )
{
    int s_fd, d1_fd, d2_fd;

    size_t sread = 1;          //  counter for # read bytes from source
    size_t d1_wrt, d2_wrt;     //  counters for # write bytes dest1 & 2 respect.

    size_t d1_cnt = 100;
    size_t d2_cnt = 50;
    char buf[ d1_cnt ];

    //  open source:
    s_fd = open ( "source.txt", O_RDONLY );
    if ( s_fd < 0 )
    {
        printf ( "'source.txt' open file error: %s\n", strerror ( errno ) );
        return 0;
    }

    //  open destination1
    d1_fd = open ( "destination1.txt", O_WRONLY );
    if ( d1_fd < 0 )
    {
        printf ( "'destination1.txt' open file error: %s\n", strerror ( errno ) );
        return 1;
    }

    //  open destination2
    d2_fd = open ( "destination2.txt", O_WRONLY );
    if ( d2_fd < 0 )
    {
        printf ( "'destination2.txt' open file error: %s\n", strerror ( errno ) );
        return 1;
    }

    while ( sread > 0 )
    {
        //  read from source
        sread = read ( s_fd, buf, d1_cnt);
        if ( sread < 0 )
        {
            printf ( "'source.txt' read #1 error: %s\n", strerror ( errno ) );
            return 1;
        }

        //  replace '1' with 'A'
        for ( size_t i = 0; i < sread ; i++ )
        {
            if ( buf[i] == '1' )
                buf[i] = 'A';
        }

        //  write to dest1:
        d1_wrt = write ( d1_fd, buf, sread );
        if ( d1_wrt < 0 )
        {
            printf ( "'destination1.txt' write error: %s\n", strerror ( errno ) );
            return 1;
        }

        sread = read ( s_fd, buf, d2_cnt );
        if ( sread < 0 )
        {
            printf ( "'source.txt' read #2 error: %s\n", strerror ( errno ) );
            return 1;
        }

        //  replace '2' with 'B'
        for ( size_t i = 0; i < sread ; i++ )
        {
            if ( buf[i] == '2' ) 
                buf[i] = 'B';
        }

        //  write to dest2:
        d2_wrt = write ( d2_fd, buf, sread );
        if ( d2_wrt < 0 )
        {
            printf ( "'destination2.txt' write error: %s\n", strerror ( errno ) );
            return 1;
        }
    }

    //  close source.txt:
    s_fd = close ( s_fd );
    if ( s_fd < 0 )
        printf ( "'source.txt' close error: %s\n", strerror ( errno ) );

    d1_fd = close ( d1_fd );
    if ( d1_fd < 0 )
        printf ( "'destination1.txt' close error: %s\n", strerror ( errno ) );

    d2_fd = close ( d2_fd );
    if ( d2_fd < 0 )
        printf ( "'destination2.txt' close error: %s\n", strerror ( errno ) );

    //  close error return handling:
    if ( -s_fd || -d1_fd || -d2_fd )
        return 1;

    return 0;
}
