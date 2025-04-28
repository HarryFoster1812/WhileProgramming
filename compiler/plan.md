Pick a file extension for the language (.while ?)

The main focus is creating a formal grammar for the while language which is already defined in the notes:


After the correctness check can be implemented using the encoding function where if an error occurs we know the code is not valid.

If it is valid then doing a multi pass compilation algorithm which will look at the variables needed.

There is no functions in the while language so none is needed.

Other useful instructions may be 
- print
- input 

Program constraints: only input and output numbers and cells
All output will be interpreted as ASCII (any number will be printed to standard output as is)

Target architecture will be ARM64/x86 since that is my laptop.
The compiler will strictly produce assembly instructions.


