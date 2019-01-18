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
#include <sys/wait.h>
int main()
{
	const int SIZE = 4096;
	const char *name = "OS";
	const char *message0= "000000000000000000000000000000000000000000 ";
	char childid[10];
	int shm_fd;
	void *ptr;
        pid_t pid,wpid;
	int status=0;
	int i;
	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	sprintf(ptr,"%s",message0);
/*	ptr += strlen(message0);
	sprintf(ptr,"%s",message1);
	ptr += strlen(message1);
	sprintf(ptr,"%s",message2);
	ptr += strlen(message2);*/
/*plan to pass name*/
	printf("name of shared memory region %s", name);
/*code to create child processes*/
/*multiple children*/
      for(i=0;i<32;i++)
      {
	/* fork a child process */
        pid = fork();

        if (pid < 0) { /* error occurred */
                fprintf(stderr, "Fork Failed\n");
                return 1;
        }
        else if (pid == 0) { /* child process */
		sprintf(childid, "%d", i);
                printf("I am the child %d %d\n",pid,i);
                execlp("/home/cslab/other/pbarnett/S18332G/Peterprogs/shm-posix-consumer","shm-posix-consumer",name,childid,NULL);
        }
	
        else { /* parent process */
                printf("I am the parent %d %d\n",pid,i);
	}
    } 
	/* parent waits for all children to complete*/
	printf("parent waiting for all children to complete\n");
	while((wpid=wait(&status))>0);
	printf("all children completed\n");
	return 0;
}
