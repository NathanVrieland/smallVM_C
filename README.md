# smallVM
A small virtual machine based on the von Neumann architecture written in C
# Usage 
write an assembly program using a small set of instructions

| Statement                       | Description                                                           |
|---------------------------------|-----------------------------------------------------------------------|
| ADD Destination Source1 Source2 | adds source1 and source2 and stores the result in destination       |
| DIV Destination Source1 Source2 | divides source1 and source2 and stores the result in destination    |
| HALT                            | halts program operation                                               |
| Identifier                      | identifiers store only 32 bit integers and start with a letter        |
| IN Variable                     | prompts console for input and stores it in variable                   |
| MUL Destination Source1 Source2 | multiplies source1 and source2 and stores the result in destination |
| OUT Value                       | outputs a variable or string literal to console                       |
| STO Destination Source          | stores Source in Destination                                          |
| SUB Destination Source1 Source2 | subtracts source1 and source2 and stores the result in destination  |

# assembly 
use assembler.py to assemble the program into smallVM bytecode 

`python3 assembler.py <assembly file> -o <output file>`

# running a program 
make sure you compile smallVM.c

`gcc smallVM.c -o smallVM`

then you can run the program 

`./smallVM <program file>`