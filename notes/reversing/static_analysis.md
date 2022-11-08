Static analysis describes the process of analyzing the code or structure of a program to determine its function. The program itself is not run at this time. Some useful steps involved in static analysis are:

- Antivirus scanning
- Finding file hashes
- Finding strings

## Packing and obfuscation
Malware obfuscation is a technique used to create textual and binary data difficult to interpret. Packing is a subset of obfuscation in which the malicious program is compressed and cannot be analyzed. Packed or obfuscated programs contain very few strings. When the packed program is run, a small wrapper program also runs to decompress the packed file and then run the unpacked file.`PEiD` is a program that can be used to detect some famous packers.

## Imports
Imports are functions used by a program that are actually stored in a different program (usually libraries). 

## Dynamic Linking vs Runtime Linking
When libraries are dynamically linked, the host OS searches for all the necessary libraries when the program is loaded. Executables that use runtime linking connect to libraries only when that function is needed. `Dependency Walker` is a program that lists the dynamically linked functions in an executable.
Some malwares can import functions by ordinals instead of name. Importing functions by ordinals makes the lives of malware analysts harder. Dependency Walker lists the names of functions corresponding to the ordinal number, thus it can be used to solve the problem of importing by ordinal.

## Function Naming Conventions
Three types of suffixes are used with function names in windows: `A`,`W` and `Ex`. Whenever Microsoft updates a function and the new function is incompatible with the old function, the new function is given the same name as the old function with a suffix `Ex`. `A` and `W` appear on functions that take a string as a parameter, `A` for ASCII strings and `W` for wide character strings.

## Exports
DLLs and EXEs export functions to interact with other programs.

## x86 disassembly
Basic static analysis does not provide enough information to analyze the malware completely so advanced static analysis techniques are used.
	Disassembly is the process of converting machine code into assembly instructions. Each instruction corresponds to an `opcode`(operation code) that tell the CPU about the operation that should be performed by the program. Disassemblers translate opcodes into human readable instructions. Operands are used to identify the data used by an instruction. Three types of operands can be used: immediate operands, register operands and memory address operands. A register is a small amount of data storage available to the CPU whose contents can be accessed more quickly than storage available elsewhere. x86 registers fall into four categories: general registers, segment registers, status flags and instruction pointers.

Let's have a look on some instructions (I've only mentioned the instructions that are a little bit confusing)

1. `mul` value
Multiplies `eax` by `value` and the result is stored as a 64-bit value across two registers: edx and eax. edx stores the most signigicant 32 bits and eax stores the least significant 32 bits.

2. `div` value
Divides the 64 bits across eax and edx by `value` (just the opposite of `mul`). The result of the division operation (quotient) is stored in the eax and the remainder is stored in edx.

