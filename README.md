HW1
Warmup: 
    (a) Extend code snippet 1 to check for read and write access permissions of a 
        given file
    (b) Write a C program where open system call creates a new file (say, 
        destination.txt) and then opens it. (Hint: use the bitwise OR flag)

Part 1
    UNIX cat command has three functions with regard to text files: displaying them,
    combining copies of them, and creating new ones. Write a C program to implement 
    a command called displaycontent that takes a (text) file name as argument and 
    display its contents. Report an appropriate message if the file does not exist
    or can’t be opened (i.e. the file doesn’t have read permission). You are to use
    open(), read(), write() and close() system calls.
    
    NOTE: Name your executable file as displaycontent and execute your program as 
    ./displaycontent file-name The cp command copies the source file specified by   
    the SourceFile parameter to the destination file specified by the 
    DestinationFile parameter.
    
Part 2
    Write a C program that mimics the cp command using open() system call to open 
    source.txt file in read-only mode and copy the contents of it to destination.txt
    using read() and write() system calls.
    
    Repeat part 2 (by writing a new C program) as per the following procedure:
        a) Read the next 100 characters from source.txt, and among characters read, 
           replace each charcter ’1’ with character ’A’ and all characters are then
           written in destination.txt
        b) Write characters "XYZ" into file destination.txt
        c) Repeat the previous steps until the end of file source.txt. The last read
           step may not have 100 characters.
        
General Instructions 
    Use perror sub-routine to display meaningful error messages
    in case of system call failures. Properly close the files using close system 
    call after you finish read/write. Learn to use man pages to know more about file
    management system calls (e.g, man read). 

========================================================================================
HW2
Part 1
    Write a program children.c, and let the parent process produce two child
    processes. One prints out 

        "I am child one, my pid is: " PID

    and the other prints out 
        
        "I am child two, my pid is: " PID

    Guarantee that the parent terminates after the children terminate (Note, you 
    need to wait for two child processes here). Use the getpid() function to 
    retrieve the PID.
    
Part 2
    Consider the parent process as P. The program consists of fork() system call
    statements placed at different points in the code to create new processes Q and
    R. The program also shows three variables: a, b, and pid - with the print out of
    these variables occurring from various processes. Show the values of pid, a, and
    b printed by the processes P, Q, and R.

========================================================================================
HW3
Part 1 
    Write a program where a child is created to execute command that tells you the 
    date and time in Unix. Use execl(...). Note, you need to specify the full path
    of the file name that gives you date and time information. Announce the 
    successful forking of child process by displaying its PID.
Part 2
    Write a program where a child is created to execute a command that shows all 
    files (including hid- den files) in a directory with information such as 
    permissions, owner, size, and when last modified. Use execvp(...). I suggest 
    that you list /usr/bin or something in /etc so I can easily run your code 
    without modifying it. Announce the successful forking of child process by
    displaying its PID.
    
Part 3
    [Step 1] Prcs_P1.c: 
    Create two files namely, destinationl.txt and destination2.txt with read, write,
    and execute permissions.

    [Step 2] Prcs_P2.c:
    Copy the contents of source.txt1 into destinationl.txt and destination2.txt as 
    per the following procedure.
        1. Read the next 100 characters from source.txt, and among characters read,
           replace each character ’1’ with character ’A’ and all characters are then
           written in destinationl.txt
        2. Then the next 50 characters are read from source.txt, and among 
           characters read, replace each character ’2’ with character ’B’ and all 
           characters are then written in destination2.txt.
        3. The previous steps are repeated until the end of file source.txt. The 
           last read may not have 100 or 50 characters.
           
   Once you’re done with successful creation of executables for the above two steps
   write a C program and call it Parent_Prcs.c. Execute the files usingexecv system
   call. Use sleep system calls to introduce delays.
   
   [Step 3] Fork a child process, say Child l and execute Prcs_Pl. This will create
   two destination files according to Step 1.
   
   [Step 4] After Child l finishes its execution, fork another child process, say
   Child 2 and execute Prcs_P2 that accomplishes the procedure described in Step 2.


CSc 332 (L) - Operating Systems Lab – Spring 2018
Task 4 - Average Grade Calculator 

Average Grade Calculator There are 10 students enrolled in a course. The course covers
x number of chapters from a textbook (x > 1). In each chapter y number of homework(s) are
assigned (y≥ 1). The average grade for each homework in all the chapters need to be found
out. To solve this, write program which has the main process as Director process, which
reads a file containing grades of all homework assignments of all chapters and creates x
number of Manager processes. Each Manager process will take care of solving a chapter. 
Each manager process will create y number of Worker process and pass one homework to each
of them and they calculate and print the average. The input file should contain the data
according to the value of x and y. The Director process is responsible for opening and 
closing the input text file. It stores the values in a two dimensional integer array with
10 rows. You may need to use the following C functions (in addition to the necessary file
& process management system calls): fopen(), fscanf(), fseek(), fclose().

Arun's explanation of the task is rather skimpy so let me explain what I want. There are
a number of ways to implement this task, but only one way to start: you need to create a
file like the illustration but containing a column for each homework for each chapter. 
Let's set a minumum of 2 homeworks per chapter and 5 chapters. That gives you a 2-d array
of 10 columns and 10 rows. The rows represent each students' grade on a homework, and the
columns represent homework assignments, where all you need to know is that there are 2 
per chapter. I would suggest that you create the input file sequentially, so that all the
values for each homework for each chapter came before the next, and then all the children
can use the same file, just starting and ending at different points (lseek). You could
also create the input file in a spreadsheet such as Excell where it would be easy to
randomize the different rows, and then extract that as a CSV file and export it to Linux.
You choose. When the "Director" parent process creates the Manager children (=5, one
for each chapter) the Director parent needs to pass the Manager children either the 
location of the input file and the offset at which to read it or else pass the actual 
values that the manager is responsible for as an array of arguments. 

Another possibility which is fine with me is to create a separate file for each Manager 
with just the quizzes for those chapters. Finally, you could choose not to overwrite the 
parent code but do some tricky ifs where if I am child 1 then I am manager and I actually
read the parent's in memory array values and so on. The process for creating workers is 
analogous. The Manager parent either gives the worker child the path of the file to open
and an offset at which to read its values, or else passes the actual values to be 
averaged as a list of parameters, or else creates a file with just the values for which 
the worker is responsible. Another way to handle this exercise is with shared memory but
we aren't quite there yet. You also have to decide how to report back and collect the 
averages, so that I know which chapter and homework the average grade represents. It is 
essential that you provide me a written explanation of your process and how you 
implemented the programs. You also need to provide me with the input file with the 
original homework grades, and an output file with the averages, labeled in some 
reasonable way, for instance a spreadsheet. Ideally, you would also provide me with a 
script or log of execution so that we can see how much parallelism is achieved by 
dividing the work up among managers and workers. 

Task 5

In this folder are a number of code examples illustrating:(1) Traditional shared-memory with semaphore management of the producer- consumer problem: ProducerConsumer.c(2) A shared memory example which does not fork or create a thread but assumes each of 2 processes knows the name of the shared memory segment. This is implemented in posix API shared memory via memory mapped file. shm_msgclient.c, shm_msgserver.c(3) A shared memory example from the textbook which forks a child and uses posix shared memory but does not use posix threads. The parent forks 32 children.(4) Another example from the textbook which creates 50 posix threads which play with the value of a passed parameter, but which does not use shared memory.Your Task 5 is to take these code examples and implement a producer-consumer solution which uses both:    Posix shared memory    Posix threads.Task 5 A. Implement the producer-consumer in such a way that the parent is the producer and the thread is the consumer and the parent and thread update the values of a queue in the parent’s memory. Are there race or deadlock conditions with this solution? Would you need to add a semaphore or mutex to protect the memory object?

Task 5 B. Implement the producer-consumer in such a way that the parent creates a shared memory segment using posix API (memory mapped file) and creates a producer thread with one routine to run, and a consumer child with another routine to run. Pass the name of the shared memory file as an argument to each thread. Does this solution require a mutex or semaphore, or does the file implementation take care of mutual exclusion? Can the producer and consumer threads write to the file at the same time?Please take note:UNIX/Linux header files required by gcc vary from one Linux to another. For example, programs that compile clean on the Debian Linux prepared for the OS book throw errors and warnings on Ubuntu in the lab!The sample programs I gave you all compile clean or with minimal warnings on Ubuntu in the lab. I had to add include files in several cases. Still warnings with the format expressions in printf statements eg printf (“%d%t....Also, when compiling with posix API functions you need to use the compile flag – lpthread, when using shared memory –lrt, and when using math functions such as max(), mod() and rand() use –lmIt is best practice (and sometimes necessary to get a compile) to put the flags at the end of the gcc statement eg.gcc myfile.c –o myfile –lpthread –lrt –lm
