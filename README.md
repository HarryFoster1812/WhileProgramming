# WHILE Compiler

A toy compiler for the WHILE programming language, implemented as an educational project to explore the basic stages of compilation — including lexical analysis, parsing, semantic analysis, and code generation.

This project also includes a *de-encoder*, which takes a natural number and decodes it into a syntactically valid WHILE program. This concept was introduced in my university's course "The Fundamentals of Computation" (COMP11212).

---

## What is WHILE?

WHILE is a simple, Turing-complete imperative programming language, frequently used in formal methods and theoretical computer science. It was introduced in *Principles of Program Analysis* by Nielson, Nielson, and Hankin.

Its intentionally minimal design makes it ideal for studying the fundamental principles of compilers.

---

## Syntax and Grammar

The syntax used here is based on a variation introduced in my university course, which differs slightly from the version in the textbook. Below is the grammar supported by this compiler:

```math
\begin{array}{l}
s ::= x := a ~|~ \text{skip} ~|~ s;s ~|~ \text{if (}b \text{) then } \{s\} \text{ else } \{s\} ~|~ \text{while (}b\text{) do } \{s\} ~|~ \text{print } x ~|~ \text{input } x \\
b ::= \text{true} ~|~ \text{false} ~|~ a == a ~|~ a \leq a ~|~ \text{!}b ~|~ b \&\& b \\
a ::= x ~|~ n ~|~ a + a ~|~ a - a
\end{array}
```
Where:

- S is a statement
- b is a boolean expression
- a is a arithmetic expression
- x is a variable
- n is a numeric value

## Language Extensions

A few modifications were made to adapt the language for practical use:

- Block syntax: The original Python-style indentation was replaced with C-style braces {} for clarity and parser simplicity.
- New instructions:
  -  print <var> — outputs the value of a variable
  -  input <var> — reads a value into a variable
- Data domain: Extended from natural numbers to all integers.

## Compiler Architecture
-  Target: x86 NASM assembly (Linux Syscalls)
-  Lexer: Character-based
-  Parser: Top-down predictive parser for statements
-  Semantic Analysis: Basic type checking and symbol table
-  Code Generator: Emits NASM assembly

## Decoder Tool

Included is a WHILE program de-encoder which takes a natural number and maps it to a valid WHILE program based on the encoding rules studied in COMP11212.
The decoder implements an encoding inspired by Gödel numbering, where syntactic constructs of a program are uniquely represented as natural numbers — illustrating how computation can be captured arithmetically.


## Running 
To run the compiler or decoder:
### Compiler
```bash
# To compile
./compiler example.while -o out.s
# To assemble
nasm -f elf64 out.s -o out.o
# Linking the object file
ld out.o -o out
# Finally run the executable
./out
```

```bash
./decode 123456789
```

## Acknowledgments
- University of Manchester — COMP11212: The Fundamentals of Computation
- Principles of Program Analysis by Nielson, Nielson, and Hankin
