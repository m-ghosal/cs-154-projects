# cs-154-projects
## Projects completed as part of the Introduction to Computer Systems (CS 154) course
Project 1: Bit Manipulation (code can be viewed in bit.c file and tested using btest.c, dlc, and driver.pl)
- Familiarized myself with the bit representation of data
- Solved a series of programming "puzzles" given certain restrictions:
  - cannot use loops or conditionals
  - can only make use of the following 8 operators: !  ̃ & ˆ | + << >>
    - some functions specifically limit the use of C arithmetic and logical operators even further
  - cannot use constants longer than 8 bits

Project 2: Defusing a Binary "Bomb" 
- This project involves “defusing” a unique “binary bomb". The bomb is an executable binary, compiled from C code, consisting of a sequence of six phases, each of which prompts you to enter a string. If you type a correct string, then the phase is defused and the bomb proceeds to the next phase. Otherwise, the bomb terminates. The bomb is defused when every phase has been defused.
- Used gdb, the GNU debugger, to reverse engineer the source for the phase 1, phase 2, ... functions (called from main())
  - traced through a program line by line, examined memory and registers, looked at both the source code and assembly code, set breakpoints, set memory watch points, and wrote scripts.

Project 3: Caching (code can be viewed in csim.c and tested using the trace files and driver.py)
- Wrote a C program that simulates the behavior of a cache memory
  - the main() function takes in takes a valgrind memory trace as input, simulates the hit/miss behavior of a cache memory (with different E, B, and S parameters) on this trace, and outputs the total number of hits, misses, and evictions.
  - uses allocate-on-write policy to handle write misses

Project 4: The Unix Shell (code can be viewed in myshell.c)
- implemented a command line interpreter or shell. When a command is typed in (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished
