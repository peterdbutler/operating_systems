//  NOTE: compile and link with -lpthread
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_SIZE 5

//  global count variables;
struct pt_args {
    int buf_count;
    char buffer[MAX_BUF_SIZE];
};

//  consumer function
void *consume( void* arg )
{
    struct pt_args *cons_arg = (struct pt_args *)arg;
    int cons_count = 0;
    int temp_char;

    while( cons_count < 26 )
    {
        if ( cons_arg->buf_count < MAX_BUF_SIZE )
        {
            //  change letter case (Upper to lower)
            temp_char = (char)cons_arg->buffer[cons_count % MAX_BUF_SIZE];
            temp_char += 32;
            cons_arg->buffer[cons_count % MAX_BUF_SIZE] = (char)temp_char;

            //  display result 
            printf ( "consumer produces: %c\n", 
                     cons_arg->buffer[cons_count % MAX_BUF_SIZE] );

            //  increment counters
            cons_arg->buf_count++;
            cons_count++;
        }
    }

    return NULL;
}

int main() 
{
    //  message to be passed from parent to thread:
    char *stream = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    //  declare struct 'prod_args' for passing argument to pthread
    struct pt_args prod_args;
    prod_args.buf_count = 5;

    //  pthread identifier for consumer thread:
    pthread_t consumer;
    
    //  create thread:
    if ( pthread_create( &consumer, NULL, consume, &prod_args ) )
        printf ( "pthread_create error: %s\n", strerror ( errno ) );

    //  producer process
    int prod_count = 0;
    while ( prod_count < 26 )
    {
        if ( prod_args.buf_count > 0 )
        {
            prod_args.buffer[prod_count % MAX_BUF_SIZE] = stream[prod_count];
            printf ( "producer produces: %c\n",
                      prod_args.buffer[prod_count % MAX_BUF_SIZE] );

            //  decrement buf_count, prod_count;
            prod_args.buf_count--;
            prod_count++;
        }
    }

    //  join thread:
    if ( pthread_join( consumer, NULL ) )
        printf ( "pthread_join error: %s\n", strerror ( errno ) );

    return 0;
}
