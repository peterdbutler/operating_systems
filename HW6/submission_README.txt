------------------------------------------------------------------------------------
TASK 6: CIGARETTE SMOKERS PROBLEM
Peter Butler
23330068
------------------------------------------------------------------------------------

Solution 1: task_6.c

This solution creates 3 pthreads, assigns each an index number in an array of posix
semaphores, and a identifying name/item from a string. This index number is passed
as the variable 'id' in the struct 'sem_args'. THe parent process, agent, then
generates a random number and wakes the associated pthread to indicate that the
needed supplies have been placed on the table (the critical sector). This random 
number is then used to awaken the associated pthread, who then processes the 
ingredients and smokes. A fourth semaphore, 'agent', is used to secure the critical 
sector and sychronize processes during the periods in which the agent is placing 
items on the table and again when the pthread with the missing item is awoken to
recieve the remaining items.

It should be noted that because the item generation is a result of the pseudo-random
number being generated, there is a chance of starvation (in the sense that a
process' identity may never be generated). Obviously if the variable definied by
ITERATIONS is a larger number, this possibility is reduced.

Because the agent wakes only the correct pthread for the items generated, no race
conditions are allowed to present themselves. Additionally, by awakening only one
pthread at a time and then immediately placing a wait on the the agent, interleaving
is impossible. After the pthread takes the items from the table, it then exits the
CS, signalling the agent in the process and the process repeats until ITERATIONS.

After ITERATIONS has been reached, the variable rand_num is set to -1, indicating
that each pthread can safely terminate and exit. The agent process then wakes,
joins, and destroys each sem/pthread, before it too destroys it's semaphore and
exits safely.

------------------------------------------------------------------------------------

Solution 2: task_6-2.c  (extra credit)

In this solution, agent does not notify the correct pthread of the items placed on
the table. Instead, it wakes all of the pthreads, who each race to gain entry to the
CS and inspect the items placed on the table. Mutual exclusion is guaranteed by the
introduction of a fifth semaphore, lock. The first pthread to get to the lock gains
entry to the CS and locks access to the other two who are forced to wait. If the
pthread is the thread needing the supplies, it removes them from the table and flags
the variable 'found', then exits the lock, signals agent, and then sleeps. 

If it is not the correct thread, a message is printed to inform the user of this 
case. Again, the process exits the lock, and the other two threads again race to 
enter. If the process had flagged the 'found' variable, each process checks and then
immediately sleeps.

This process guantees each pthread checks the table, eliminating the possibility of
starvation. Additionally, the lock semaphore guantees that interleaving is
impossible while inspecting the CS.

with the exception of the above changes, the program is the same as the first.
------------------------------------------------------------------------------------


