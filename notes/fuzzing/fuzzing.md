## Fuzzing
Fuzz testing or fuzzing is a black box software testing technique that involves feeding random inputs to a computer program to find software vulnerabilities. The program is monitored for exceptions such as crashes, built-in assertions, or potential memory leaks. 

## Deterministic and Non Deterministic Algorithms
In a deterministic algorithm, for a given input, the computer will always produce the same output going through the same states whereas in non deterministic algorithms, for the same input, the computer may produce different output in different runs. They cannot solve problems in polynomial time and cannot determine the next step.

## Mutation based fuzzing
Mutation based fuzzing consists of altering a sample file or data following specific heuristics.

## Coverage Guided Fuzzer
It gathers coverage information for each mutated input in order to discover new execution paths and potential bugs.

## Instrumentation
A technique to measure code coverage by adding markers to code blocks. Every fuzzer has its own method for injecting instrumentation into the target application.

## Sanitizers
Detection tools based on compiler instrumentation.


## American Fuzzy Lop
A fuzzer that employs genetic algorithms in order to efficiently increase code coverage of the test cases. It uses a non-deterministic testing algorithm, so two fuzzing sessions are never the same. Setting the same seed gives similar results. To enable instrumentation, the application should be compiled with AFL's compilers.

## Installing AFL++
```sh
sudo apt-get update
sudo apt-get install -y build-essential python3-dev automake git flex bison libglib2.0-dev libpixman-1-dev python3-setuptools
sudo apt-get install -y lld-11 llvm-11 llvm-11-dev clang-11 || sudo apt-get install -y lld llvm llvm-dev clang 
sudo apt-get install -y gcc-$(gcc --version|head -n1|sed 's/.* //'|sed 's/\..*//')-plugin-dev libstdc++-$(gcc --version|head -n1|sed 's/.* //'|sed 's/\..*//')-dev

cd $HOME
git clone https://github.com/AFLplusplus/AFLplusplus && cd AFLplusplus
export LLVM_CONFIG="llvm-config-11"
make distrib
sudo make install
```
When source code is available, AFL++ can use instrumentation, inserting function calls at the beginning of each basic block(functions, loops,etc.). To enable instrumentation, we need to compile code with AFL's compilers.

## Compiling a target application using AFL'S compilers

```
export LLVM_CONFIG="llvm-config-11"
CC=/path/to/afl-clang-fast CXX=/path/to/afl-clang-fast++ ./configure --prefix="target/path/for/installation"
make
make install
```

## afl-clang-lto
Provides collision-free instrumentation that is faster and provides better results than afl-clang-fast.
LTO stands for Link Time Optimization.

## Path Collisions
They prevent fuzzers from discovering potential paths that lead to new crashes.

## ASan (Address Sanitizer)
ASan is an open source tool that detects memory corruption bugs. It detects errors at runtime. AFL++ supports ASan.





References
[https://www.geeksforgeeks.org/difference-between-deterministic-and-non-deterministic-algorithms/](https://www.geeksforgeeks.org/difference-between-deterministic-and-non-deterministic-algorithms/)