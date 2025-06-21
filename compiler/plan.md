Pick a file extension for the language (.while ?)

Although the language is defined to only work in the domain of the natural numbers, for ease of implementation i have decided that it shall be extended to the domain of integers

I have no plan to expand to further to encompass the entirety of the computational number line

The main focus is creating a formal grammar for the while language which is already defined in the notes:

If it is valid then doing a multi pass compilation algorithm which will look at the variables needed.

There is no functions in the while language so none is needed.

Other useful instructions may be

- print
- input

Program constraints: only input and output numbers and cells
All output will be interpreted as ASCII (any number will be printed to standard output as is)

Target architecture will be x86_64 since that is my laptop.
The compiler will strictly produce assembly instructions.

# Phases

## main.c

- takes in the cmd line arguments
- calls the pipleline

## 1 - lexer.c

- Tokenise the input
- Contains TokenType struct, Token struct

## 2 - parser.c

- takes the tokens and parses them into the abstract syntax tree
- implement recursive decent parsing

## 3 - ast.c

- Defines the node types (WHILE, IF, Assign, )

## 4 - codegen.c

- Walks the ast and generates assembly code
- write code to output file

## Util/Helper

### symtab.c

Symbol table for keeping a list of variables (could implement with a array or hashmap)
map variable names to memory addresses (all variables are initialied to 0 so it makes it quite easy)

### Util.c

Error messages, file loading, string handling
Helper macros like expect_token(), panic(), etc.

main.c
 └── lexer.c
       └── parser.c
             └── ast.c
                   └── codegen.c → outputs assembly

Is there a need for an optimiser?

# Syntax of the language

The syntax of the language follows the Grammar:
$$
\begin{array}{left}
s ::= x := a | skip | S_{1};S_{2}|\text{if b then } S_{1} \text{ else } S_{2} | \text{while } b \text{ do } s | print x | input x \\
b ::=true | false | a_{1}=a_{2}|a_{1}\leq a_{2} | \lnot b|b_{1} \land ~  b_{2} \\
a ::= x|n|a_{1}+a_{2}|a_{1}-a_{2}| a_{1} * a_{2}
\end{array}
$$

Where:

- S is a statement
- b is a boolean expression
- a is a arithmetic expression
- x is a variable
- n is a numeric value

PLAN FOR DEVELOPMENT STEPS:

- Create the basics
- Statically assign each variable to the .data section (32 bits of memory for each and initalised to 0)
- Have full memory access in each operation eg:
mov x(%rip), %eax
add $1, %eax
mov %eax, x(%rip)
- ADD ERROR HANDLING (In the lexing stage add a line and col number attribute for easy error handling/debugging)

(inefficent but do not need to handle register tracking)

Later on:

- Constant folding eg. 9+2 or 1*9+4 ect
- Variable cache (minimise load store)
