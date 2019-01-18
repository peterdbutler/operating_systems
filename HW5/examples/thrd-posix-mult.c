
#include </usr/include/pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define NUM_THREADS 50
int sum,i,j; /* this data is shared by the thread(s) */
int tidlist[50];
void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
pthread_t workers[NUM_THREADS]; /* the thread identifier */
pthread_attr_t attr; /* set of attributes for the thread */
if (argc != 2) {
	fprintf(stderr,"usage: a.out <integer value>\n");
	/*exit(1);*/
	return -1;
}

if (atoi(argv[1]) < 0) {
	fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
	/*exit(1);*/
	return -1;
}

/* get the default attributes */
pthread_attr_init(&attr);
/*initialize tidlist*/
for (i=0;i<50;i++) tidlist[i]=0;
/* create the threads */
for(i=0;i<atoi(argv[1]);i++) 
pthread_create(&workers[i],&attr,runner,argv[1]);

/* now wait for the thread to exit */
for(i=0; i<atoi(argv[1]); i++)
pthread_join(workers[i],NULL);

printf("parent sum = %d\n",sum);
for(j=0;j<50;j++) printf("%d", tidlist[j]);
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param) 
{
int i, upper = atoi(param);
int myid = rand()%upper;
sum = 0;

	if (upper > 0) {
		for (i = 1; i <= upper; i++)
			sum += i;
	}
tidlist[myid]=sum;
printf("I am the thread child %d\n", myid);
for(j=0;j<50;j++) printf("%d%t", tidlist[j]);
	

	pthread_exit(0);
}
