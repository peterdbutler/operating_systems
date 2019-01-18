#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( int argc, char *argv[] )
{
    //  CSV file variables
    int students = 10;
    int chapters = 5;
    int quiz = 2;

    //  read/write variables:
    char *readname;

    //  forking variables:
    int status, manager, worker;

    //  Usage warning:
    if ( argc != 2 )
    {
        if ( argc < 2 )
            printf ( "Too few arguments provided!\nUSAGE: %s filename\n%s", argv[0],
                     "NOTE: filename must exist and be readable\n" );
        else
            printf ( "Too many arguments provided!\nUSAGE: %s filename\n%s",
                argv[0], "NOTE: filename must exist and be readable\n" );
        return 0;
    }

    FILE * source = fopen ( argv[1], "r" );

    //  fork and write in parallel!
    for ( int x = 0; x < chapters; x++ )
    {
        //  start of conv code:
        size_t max_line_size = 1024;
        char line[max_line_size];
        int grades[( quiz*students )];
        int grades_index = 0;

        char *saveptr, *token;

        //  convert csv file into a series of arrays
        for ( int y = 0; y < quiz; y++ )
        {
            if ( fgets ( line, max_line_size, source ) == NULL )
            {   // error message:
                printf ( ".csv conversion error: fgets failed\n" );
                return 1;
            }

            //  convert line into array of ints:
            token = strtok_r ( &line[0], ",", &saveptr );
            for ( int j = 0; j < students; j++ )
            {
                grades[grades_index] = atoi ( token );
                token = strtok_r ( NULL, ",", &saveptr );
                grades_index++;
            }
        }

        manager = fork();
        if ( manager < 0 )
            printf ( "m%i fork error: %s\n", ( x+1 ), strerror ( errno ) );
                     
        //  manager only tasks:
        if ( manager == 0 )
        {
            printf ( "m%i forked from director\n", x+1 );

            //  worker forking and assignment...
            for ( int y = 0; y < quiz; y++ )
            {
                //  create pipe for worker - manager communications
                int pfd[2];
                float pbuf;

                if ( pipe( pfd ) < 0 )
                {   // error message:
                    printf ( "pipe: m%i.w%i create error: %s\n",
                              x+1, y+1, strerror ( errno ) );
                }

                //  fork & pass grades array to worker #'y'
                worker = fork();
                if ( worker < 0 )
                {   // error message:
                    printf ( "m%i.w%i fork error: %s\n", x+1, y+1, 
                             strerror ( errno ) );
                }

                //  worker-only tasks
                if ( worker == 0 )
                {
                    printf ( "m%i.w%i: ..calculating avg grade for Ch%i Q%i\n",
                             x+1, y+1, x+1, y+1 );
                              
                    //  workeer averages calculations: 
                    float avg = 0;
                    for ( int i = (0+y*students); i < ( students+y*students ); i++ )
                        avg += grades[i];
                    avg = ( avg/students );

                    // pipe avg up to manager
                    pbuf = avg;
                    close ( pfd[0] ); // close read end of pipe
                    if ( write ( pfd[1], &pbuf, sizeof ( pbuf) ) < 0 )
                        printf ( "pipe: m%i.w%i write error: %s\n", 
                                  x+1, y+1,  strerror ( errno ) );
                    close ( pfd[0] );
                    return 0;
                }

                //  manager pipe end:
                close ( pfd[1] ); // close write end of pipe
                if ( read ( pfd[0], &pbuf, sizeof ( pbuf ) ) < 0 )
                {
                    printf ( "pipe: m%i.w%i read error: %s\n", 
                             x+1, y+1,  strerror ( errno ) );
                }
                close ( pfd[0] ); // close read end of pipe

                printf ( "m%i: pipe contains avg grade for: Ch%i, Q%i = %.2f\n",
                         x+1, x+1, y+1, pbuf );
            }
            return 0;
        }
    }

    waitpid ( manager, &status, 0 );
    printf ( "\ndirector process quiting.\n" );
    return 0;
}
