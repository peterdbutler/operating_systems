HW1
Warmup: 
    (a) Extend code snippet 1 to check for read and write access permissions of a 
        given file
    (b) Write a C program where open system call creates a new file (say, 
        destination.txt) and then opens it. (Hint: use the bitwise OR flag)

Task 1
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
    
Task 2
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
CSc 332 (L) - Operating Systems
Excel where it would be easy to randomize the different rows, and then extract that as a CSV file and export it to Linux. You choose.

In this folder are a number of code examples illustrating:

Task 5 B. Implement the producer-consumer in such a way that the parent creates a shared memory segment using posix API (memory mapped file) and creates a producer thread with one routine to run, and a consumer child with another routine to run. Pass the name of the shared memory file as an argument to each thread. Does this solution require a mutex or semaphore, or does the file implementation take care of mutual exclusion? Can the producer and consumer threads write to the file at the same time?