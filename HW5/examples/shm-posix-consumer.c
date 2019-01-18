/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include </usr/include/pthread.h>
#include <unistd.h>
#include <sys/types.h>

int main(argc,argv)
	int argc;
	char *argv[];
{
/*	const char *name = "OS";*/
	const int SIZE = 4096;

	int shm_fd;
	void *ptr;
	int i,j,childpid,idle;
	char modstring[50];
        /*test parameter passing*/
	printf("name of segment passed = %s\n", argv[1]);
	/* open the shared memory segment */
	printf("child id= %s\n", argv[2]);
	childpid=atoi(argv[2]);
	shm_fd = shm_open(argv[1], O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("child open shared memory rw failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Child map with read write failed\n");
		exit(-1);
	}

	/* now read from the shared memory region */
	/* now test writing to shared memory*/
        printf("child id %s sees shared memory before writing %s\n",argv[2],ptr);
/*debug first copy the string to a character array*/
	sprintf(modstring,"%s",ptr);
/*debug look for a 0 character and replace it with 1*/
	for (j=0;j<50;j++)
	if(modstring[j]=='0')
		{modstring[j]='1';
		 printf("child id %s got pid %d", argv[2], j);
		 childpid=j;
		 break;
		}
/*debug put modstring back in shared memory*/
	
	printf("debug modstring child %s %s\n",argv[2], modstring);
	sprintf(ptr,"%s", modstring);
	idle=rand() % 32;
	sleep(idle);
	/* now put it back*/
	sprintf(modstring,"%s",ptr);
	modstring[childpid]='0';
	printf("child %s returned pid %d mem= %s\n", argv[2],childpid,ptr); 
	sprintf(ptr,"%s", modstring);
	/* dont remove the shared memory segment 
	if (shm_unlink(argv[1]) == -1) {
		printf("Error removing %s\n",argv[1]);
		exit(-1);
	}*/

	return 0;
}
