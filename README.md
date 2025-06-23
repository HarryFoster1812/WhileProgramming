A toy compiler for the WHILE programming language, implemented as an educational project to learn and implement the basic stages of compilation including parsing, semantic analysis, and code generation. As well as a compiler i also implemented a de-encoder which takes a natural number as input as de-encodes it into a valid while program which the rules were defined with my University's course unit "The Fundamentals of Computation", unit code: COMP11212

# What is WHILE?

WHILE is a simple, Turing-complete imperative programming language often used in formal methods and theoretical computer science. The language was introduced in "Principles of Program Analysis" by Nielson, Nielson, and Hankin.  

The language is intentionally minimal, making it ideal for studying the core principles of compilers.

# Syntax and Language Design

Personally I was introduced to the WHILE language via my University course where they had slightly different syntax to what was defined in the textbook. I have used my University's grammar as my inspiration for the grammar of this version of the while language.

For those who can read a grammar this is the grammar which the compiler uses:

```math
\begin{array}{left}
s ::= x := a ~|~ \text{skip} ~|~ s;s ~|~ \text{if (}b \text{) then } s \text{ else } \{s\} ~|~ \text{while } (b) \text{ do } \{s\} ~|~ \text{print } x ~|~ \text{input } x \\
b ::= \text{true} ~|~ \text{false} ~|~ a==a ~|~ a\leq a ~|~ \text{!} b ~|~ b \text{\&\&} b \\
a ::= x~|~n~|~a+a~|~a-a
```

Where:

- S is a statement
- b is a boolean expression
- a is a arithmetic expression
- x is a variable
- n is a numeric value

## Language Changes

Since in the first semester we were introduced to python as our first language the original grammar featured indentation as the basis for blocks. I have replaced this in favour of c-style syntax featuring braces and parentheses.

In addition, i have also introduced the instructions print <var> and input <var> since the original language lacked these.

I also made the decision to extend the domain of the language to be the set of integers instead of the intended natural numbers.

# Compiler Details

## Target

The compiler targets x86 NASM assembly.

## Implementation Details

- Character based Lexer
- A top-down predictive parser (for statements)
