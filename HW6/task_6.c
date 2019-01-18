//  NOTE: compile & link with -lpthread
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_SUPPLIES 3
#define ITERATIONS 10

//  global array to identify p_thread by possessions
const static char *items[3] = { "TOBACCO", "PAPER", "MATCH" };

//  struct to pass semaphores as args to p_threads
struct pt_args {
    sem_t *agent;
    sem_t *sem_smokers[NUM_SUPPLIES];
    int table[NUM_SUPPLIES];
    int rand_num;
    int id;
};

//  smoke decleration
void *smoke( void *arg );

int main()
{
    //  declare struct for p_thread args
    struct pt_args sem_args;

    //  create semaphore 'agent'
    sem_args.agent = sem_open ( "agent", O_CREAT | O_EXCL, 00666, 0 );
    if ( sem_args.agent == SEM_FAILED )
    {
        printf ( "semaphore 'agent' create error: %s\n", strerror ( errno ) );
        return -1;
    }

    //  init default table, create semaphores, create p_threads
    pthread_t pt_smokers[NUM_SUPPLIES];
    for ( int i = 0; i < NUM_SUPPLIES; i++ )
    {
        //  init default table
        sem_args.table[i] = 0;
        
        //  create semaphore for ea. value in items (i.e. tobacco, paper, matches)
        sem_args.sem_smokers[i] = sem_open ( items[i], O_CREAT | O_EXCL, 00666, 0); 
        if ( sem_args.sem_smokers[i] == SEM_FAILED )
        {
            printf ( "%s semaphore create error: %s\n", items[i], strerror(errno) );
            return -1;
        }

        //  create smoker[i] p_thread, pass identity as argument to ea. p_thread
        sem_args.id = i;
        if ( pthread_create ( &pt_smokers[i], 0, smoke, &sem_args ) )
        {
            printf ( "pthread %i creation error: %s\n", i, strerror ( errno ) );
            return -1;
        }

        printf( "...init pthread %i...\n", i );

        //  block
        if ( sem_wait ( sem_args.agent ) )
            printf ( "WARNING: sem_wait(agent) error: %s\n", strerror ( errno ) );
    }

    //  agent process
    for ( int i = 0; i < ITERATIONS; i++ )
    {
        // generate random number
        sem_args.rand_num = rand() % (NUM_SUPPLIES);
        printf ( "\n" );

        // place item on table
        for ( int i = 0; i < NUM_SUPPLIES; i++ )
        {
            if ( i != sem_args.rand_num )
            {
                sem_args.table[sem_args.rand_num] = 1;
                printf ( "Agent placed %s on the table...\n", items[i] );
            }
        }

        //  wake target smoker
        if ( sem_post ( sem_args.sem_smokers[sem_args.rand_num]) )
            printf ( "WARNING: sem_post(%s) error: %s\n", 
                     items[sem_args.rand_num], strerror ( errno ) );

        //  P(agent)    -- agent sleeps
        if ( sem_wait ( sem_args.agent ) )
            printf ( "WARNING: sem_post(lock) error: %s\n", strerror ( errno ) );
    }

    //  send terminate signal to smoker processess
    sem_args.rand_num = -1;

    //  ordered awaken sem, pthread join, & sem destroy
    for ( int i = 0; i < NUM_SUPPLIES; i++ )
    {
        //  awaken
        if ( sem_post ( sem_args.sem_smokers[i] ) )
            printf ( "WARNING: sem_post(%s) error: %s\n", items[i],strerror(errno));

        //  join:
        if ( pthread_join ( pt_smokers[i], 0 ) )
            printf ( "WARNING: pthread #%i ('%s') join error: %s\n", i+1, items[i], 
                     strerror ( errno ) );

        //  destroy/unlink sem
        if ( sem_unlink ( items[i] ) )
            printf ( "WARNING: semaphore #%i ('%s') destroy error: %s\n",
                      i+1, items[i], strerror ( errno ) );
    }

    //  destroy agent before exit
    if ( sem_unlink ( "agent") )
        printf ( "WARNING: semaphore 'agent' destroy error: %s\n", strerror(errno));
    return 0;
}

//  smoke definition
void *smoke( void *arg )
{
    //  init
    struct pt_args *sem_args = (struct pt_args*)arg;
    int id = sem_args->id;

    printf ( "I am process #%i, I am the smoker with %s\n", id+1, items[id] );

    //  block immediately after init
    if ( sem_post ( sem_args->agent ) )
        printf ( "WARNING: sem_post(agent) error: %s\n", strerror ( errno ) );
    if ( sem_wait ( sem_args->sem_smokers[id] ) )
        printf ( "WARNING: sem_wait(%s) error: %s\n", items[id], strerror(errno) );

    // pick up items
    while ( sem_args->rand_num != -1 )
    {
        if ( sem_args->rand_num == id )
        {
            for ( int i = 0; i < NUM_SUPPLIES; i++ )
            {
                if ( i != id )
                {
                    sem_args->table[id] = 0;
                    printf ( "\t\t\t\t\t...the smoker w/ %s picked up %s\n",
                              items[id], items[i] );
                }
            }
            printf ( "\t\t\t\t\t...the smoker w/ %s is busy smoking.\n", items[id]);

            // wake agent & block
            if ( sem_post ( sem_args->agent ) )
                printf ( "WARNING: sem_post(agent) error: %s\n", strerror(errno) );
            if ( sem_wait ( sem_args->sem_smokers[id] ) )
                printf ( "WARNING: sem_wait(%s) error: %s\n", items[id],
                          strerror(errno) );
        }
    }
    return NULL;
}
