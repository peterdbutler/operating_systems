Peter Butler
23330068
task5 - producer-consumer

TASK 5A:

There are two implementations for this task. Task_5a_1 passes a single char pointer
from the producer process to a created consumer p_thread. Task_5a_2 passes a pointer
to a char array from the producer process to a created consumer p_thread.

In each version, producer is given an input string ('stream' - some sample input, in
this case the capitalized alphabet). Producer then iterates over this input and
passes it, char by char to consumer. The consumer p_thread calls the function
consume with a single arg (the pointer to either a char or a char array). This
function adds 32 to the value stored at the address passed to it, changing the case
of the letter from upper to lower (simply as a demonstration that the consumer
process is doing something).

There are race conditions on the variable stream_count, as each process can operate
on it. However, I think I've implemented the solution in such a way that I've
pseduo-locked it. I.e. each process only checks to see it's current value, neither
will interact with the memory segmet unless mutually exclusive conditions are
satisfied.

I think for the first version of task_5a a mutex should be used, and for the second
version a counting semaphore would be appropriate.

TASK 5B:

This function creates a shared memory segment, and then passes a pointer to the 
segment to two p_threads, consumer and produce. Producer calls the produce function,
which generates a string of chars, again, the uppercase alphabet, and writes each
char to the shared memory. Consumer calls the function consume, and it iterates over
the values stored in the shared memory segment, again adding 32 to the value of
each, changing the case from upper to lower.

There are race conditions on gaining access to the shared memory segment for each
p_thread. However, once a process gains access to the memory, POSIX maintains that
only one can write at a time, and so the race looser other is blocked. As currently
implemented, there is no set order to which process access the memory first. As
such, consumer could complete it's function before producer, and it would appear
than consumer had not run, as producer would write over whatever consumer had
changed. I would propose using a semaphore to indicate which process should operate
on the shared memory segment first.
