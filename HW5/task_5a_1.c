//  NOTE: compile and link with -lpthread.
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

//  global count variables;
int stream_count, buf_count;

//  consumer function
void *consume( void* arg )
{
    int *buffer = ( int *)arg;
    while( stream_count < 26 )
    {
        if ( buf_count == 1 )
        {
            //  change letter case (Upper to lower)
            *buffer += 32;
            printf ( "consumer produces: %c\n", ( char )*buffer );

            //  decrement buf_count
            buf_count = 0;
            stream_count++;
        }
    }

    return NULL;
}

int main() 
{
    //  initialize counts
    stream_count = 0;
    buf_count = 0;
    
    //  producer object to be passed to consumer
    char buffer;

    //  message to be passed from parent to thread:
    char *stream = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    printf ( "message to be passed from parent (producer) to thread (consumer):\n");
    for ( int i = 0; i<26; i++ )
        printf( "%c", stream[i] );
    printf( "\n" );

    //  pthread identifier for consumer thread:
    pthread_t consumer;
    
    //  create thread:
    if ( pthread_create( &consumer, NULL, consume, &buffer ) )
        printf ( "pthread_create error: %s\n", strerror ( errno ) );

    //  producer process
    int pro_count = 0;
    while ( pro_count < 26 )
    {
        if ( buf_count == 0 )
        {
            buffer = *( stream + stream_count );
            printf ( "producer produces: %c\n", buffer );

            buf_count++;
            pro_count++;
        }
    }

    //  join thread:
    if ( pthread_join( consumer, NULL ) )
        printf ( "pthread_join error: %s\n", strerror ( errno ) );

    return 0;
}
