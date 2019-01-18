//  NOTE: compile and link with -lpthread -lrt
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

//  consumer function
void *consume( void* arg )
{
    //  get shm_ptr from arg
    char *shm_ptr = (char *)arg;

    //  change case of shared memory from upper to lower
    for (int i = 0; i < 26; i++ )
    {
        printf ( "consumer consumes: %c\n", shm_ptr[i] );    
        shm_ptr[i] += 32;    
    }

    return NULL;
}

void *produce( void* arg )
{
    //  get shm_ptr from arg
    char *shm_ptr = (char *)arg;

    //  message to be passed from parent to consumer:
    char *stream = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //  place stream string into shared memory 
    for (int i = 0; i < 26; i++ )
    {
        shm_ptr[i] = stream[i];    
        printf ( "producer produces: %c\n", shm_ptr[i] );    
    }

    return NULL;
}

int main() 
{
    //  shared memory variables
    char *shm_name = "/shm";
    const int shm_size = 256;
    int shm_fd;

    //  create shared memory file
    shm_fd = shm_open ( shm_name, O_CREAT | O_RDWR, 0666 );
    if ( shm_fd < 0 ) 
    {
        printf ( "shm creation error: %s\n", strerror ( errno ) );
        return 1;
    }

    //  resize shared memory file
    if ( ftruncate ( shm_fd, shm_size ) )
    {
        printf ( "shm ftruncate error: %s\n", strerror ( errno ) );
        return 1;
    }
    
    //  shared memory pointer (to be passed to ea. thread)
    void *shm_ptr;
    
    //  create shm mapping
    shm_ptr = mmap ( 0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0 );
    if ( shm_ptr < 0 )
    {
        printf ( "shm mapping failed: %s\n", strerror ( errno ) );
        return 1;
    }

    //  pthread identifier for consumer thread:
    pthread_t consumer;
    pthread_t producer;
    
    //  create consumer thread:
    if ( pthread_create( &consumer, NULL, consume, shm_ptr ) )
        printf ( "consumer pthread_create error: %s\n", strerror ( errno ) );

    //  create producer thread:
    if ( pthread_create( &producer, NULL, produce, shm_ptr ) )
        printf ( "producer pthread_create error: %s\n", strerror ( errno ) );

    //  consumer join thread:
    if ( pthread_join( consumer, NULL ) )
        printf ( "consumer pthread_join error: %s\n", strerror ( errno ) );

    //  producer join thread:
    if ( pthread_join( producer, NULL ) )
        printf ( "producer pthread_join error: %s\n", strerror ( errno ) );
    
    //  print the new string:
    char *char_ptr = ( char* )shm_ptr;
    printf ( "the final shared memory segment:\n" );
    for ( int i = 0; i<26; i++ )
        printf ( "%c", char_ptr[i] );
    printf ( "\n" );

    // close shared mem
    if ( close ( shm_fd ) )
    {
        printf ( "shared mem close error: %s\n", strerror ( errno ) );
        return 1;
    }
    return 0;
}
