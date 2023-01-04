---
layout: page
title: v8 Internals
comments: true
tags: [browser,v8,pwn]
---

### Isolate
Isolate represents an isolated instance of the V8 engine. v8 isolates have completely separate states. It includes heap manager, garbage collector, etc. Only one thread may access a given isolate at a time, but different threads may access different isolates simultaneously. Objects from one isolate must not be used in other isolates.


### Context
A context is nothing but an execution environment that allows separate JavaScript applications to run in the same isolate.


### Implementation of the JS standard library
There are various ways to implement the JS standard library such as the C++ API, assembly and JavaScript itself. But each of these methods has some bad effect.

* Using the C++ API
C++ is a high level language, it has compiler optimizations, it is fast which makes it a good choice for implementing the standard library. However, C++ is not good at garbage collection and v8 is a garbage collected engine, the objects in v8 are garbage collected. Objects on the v8 heap can move anytime which must be handled with care which makes C++ a bad choice for it. Making transition from JS to C++, checking different calling conventions,etc. is expensive. This shouldn't be done for functions that should be called quickly.

* Using assembly
Obviously, assembly is fast but it is prone to errors.

* Using JavaScript
JavaScript is easily readable, compatible with v8's garbage collector. However, JS semantics can be exploited to get memory leaks. Exposing the VM internal structs is necessary to increase the execution speed. But, exposing internal structures to a user controlled language like JS would make the browser vulnerable to memory leaks.

* Using v8's optimizing compiler
The optimizing compiler can access the internal structs without exposing them to external users, provides low-level control, provides a good control on semantics. In order to use the optimizing compiler for implementing the library, the developers will need to write code in IR which is a low-level language and difficult to read and write.

* CodeStubAssembler(CSA)
It is a C++ API used to generate IR for v8's optimizing compiler which is then compiled into machine code for the target machine. It is based on the backend of Turbofan, v8's optimizing compiler.

* Torque
It is a V8-specific domain-specific language built on the top of CSA. It is statically typed, garbage collected, compatible with JavaScript and easily readable. The Torque compiler doesn't create machine code directly, but generates C++ code that calls V8's existing CodeStubAssembler interface.

### Torque Macros
Macros are just like inline functions. They are used to remove the effect of call overhead.


### Transient Types
In v8,heap objects can change layout at runtime. To express object layouts that are subject to change or other temporary assumptions in the type system, torque supports the concept of transient types.

References:

[https://v8.github.io/api/head/index.html](https://v8.github.io/api/head/index.html)
[https://youtu.be/YcroNGHHqtU](https://youtu.be/YcroNGHHqtU)
[https://github.com/danbev/learning-v8](https://github.com/danbev/learning-v8)