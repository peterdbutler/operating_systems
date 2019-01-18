Peter Butler
23330068
csc332: task4

USAGE: Please use the provided grades.csv file. 
    I view each row of the file as the quiz grades for a particular chapter, with 2
    quizes per chapter

------------------------------------------------------------------------------------
Forking Scheme:
------------------------------------------------------------------------------------

The director will fork X managers, where X == the number of chapters for the course.
Before forking each manager, the director opens the argument file, grades.csv, which
will provide a list of quiz grades for each chapter. There are 5 chapters, and 2
quizes each chapter. Each row represents a quiz for a chapter, in increasing order.

Before forking a manager, the director reads the top Y lines from the file, where Y
== the number of quizes per chapter. These lines are transformed from csv format and
placed into an array of ints, to be inherited by each manager as it forms. The
director will maintain the offset in the original fd, so that as each manager is
forked off, the director will increment down the file providing new input at each
fork.

Each manager is then tasked with forking Y workers, where Y == the number of quizes
per chapter. These workers will iterate over 10*(1+y) indices in the array, so that,
while each worker receives the same copy of the array, they will operate on
different sections. 

Each worker is in charge of computing the average value for each quiz. This value is
then saved in the variable 'avg' and piped up to the parental manager, who then
outputs the value to the command line, reporting the ch and quiz number identifying
the average grade.

Additionally, various outputs are provided, indicating (approximately) at what stage
the director and each manager and worker are at, illustrating the degree of
paralellism at work in the program.


As an aside...

I would have liked to incorporate a 'write' scheme, perhaps piping the averages
further up the tree to the director, to be written into a file. Unfortunately this
portion is left incomplete, as I began to battle with the kernal's scheduler and
couldn't reliably get results from certain children as needed.... 
