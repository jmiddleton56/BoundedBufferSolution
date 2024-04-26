# BoundedBufferSolution

This is a simple example of my solution to the bounded buffer problem. 

 ** Compilation Instructions **
 To compile and run this program, you must use GCC.
 Simply compile the Bounded_Buffer.c file with
 the command "gcc -o BB Bounded_Buffer.c -lpthread -lrt", and
 then execute the program with "./BB x y z"
 where 'x', 'y', and 'z' are the 3 arguments provided
 that represent the sleep time, # of producers, 
 and # of consumers.

 ** Steps **
 1. Compile with the command "gcc -o BB Bounded_Buffer.c -lpthread -lrt".
 2. Run the executable with "./BB x y z".

 ** Example ** 
 Sleep Time : x = 10
 # of producers : y = 2
 # of consumers : z = 3

 To execute the file with these values, run the command as follows:
 "./BB 10 2 3"
