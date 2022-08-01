---
layout: page
title: Browser pwn
comments: true
tags: [browser]
---

## Web Browser
An application used to locate, retrive and display content on the World Wide Web (WWW). Important components of a web browser are User Interface, browser engine, rendering engine, networking, data storage and JS interpreter.

### Rendering Engine
The rendering engine is repsonsible for rendering a webpage on the screen. It parses HTML, CSS and displays the content on the screen. The networking layer sends the contents of the requested documents to the rendering engine in chunks of 8 kilobytes. After that, the rendering engine converts the html elements to DOM nodes in a tree known as the DOM Tree (or content tree). Meanwhile, the browser creates another tree known as the render tree which stores the order of elements in which they will be displayed. In case of firefox, the nodes are called frames. The render tree goes through the layout process (reflow) which calculates coordinates for each node. After that, the render tree is traversed and the renderer's `paint()` method is called to display content on the screen.

### Browser Engine
It handles interactions between the user interface and the rendering engine. 

## Data storage
You might have seen IndexedDB, Local Storage, Session storage and WebSql in a browser. Browsers store data in the local file system.

## JavaScript Engine(or JavaScript Interpreter)
A JS engine is a computer program that converts JS into low level machine code. Some famous JS engines are:
V8(Chrome), Spidermonkey(Firefox), JS Core Webkit (Safari), Chakra(Edge)
A JS engine is made up of various components such as parser, Abstract syntax tree(AST), interpreter, profiler, compiler, etc.

### Parser
A parser reads the code line by line and checks the syntax, errors,etc. It is responsible for performing lexical analysis that converts code into a set of tokens describing different parts of code. Further, these tokens are used to create the abstract syntax tree.

### Abstract Syntax Tree (AST)
An AST is nothing but a tree representation of source code written in a formal language. It represents the actual structure of the source code.

### Interpreter
The interpreter walks through the AST and generates bytecode. The AST is deleted once the bytecode has been generated.

### Profiler
It is responsible for performing code optimization. It keeps a track of the types used and the number of times the code is run. 

### Compiler
It creates a translation of the code that has been written and compiles down to a lower level language that can be read by a machine. 

### JIT (Just-In-Time) Compiler
In languages such as C and C++, compilation is done ahead of time(AOT) i.e the entier code is compiled before execution. But, the JIT compiler works a little bit differently. In case of JIT, compilation is done during execution. A great thing about JIT compiler is that once code starts running, it is able to optimize it. 
The monitor(profiler) runs the code through the interpreter and keeps a track of the number of times various segments of code are run. If the number grows to a considerable limit, the segment is marked as warm and sent to the baseline compiler. The segment is marked as hot if the number becomes too high. 
The baseline compiler converts the warm sections of code into optimized bytecode which runs faster. Each instruction is converted into a stub by the baseline compiler which are indexed by line number and variable type. 
Hot sections of code are sent to the optimizing compiler which further optimizes the code. It organizes stubs to form groups. After optimization, execution is passed to the interpreter or baseline compiled version. This process is known as deoptimization. 
V8 uses the TurboFan optimizing compiler. SpiderMonkey uses IonMonkey JIT compiler. Also, the interpreter in V8 is knows as Ignition.

One major difference between the interpreter and the compiler is that the interpreter translates the code one statement at at time while the compiler reads the entire code and then converts it. Overall execution time is lower in case of compilers.


### Execution 
The bytecode is executed by using the memory heap and the call stack of the V8 engine's runtime environment. Memory heap is used to assign memory to all variables and functions. Functions are pushed to the stack when the are called and popped from the stack when their execution is complete.

Most JavaScript interpreters use dictionary-like objects to store the location of object property values in memory. In case of JavaScript, several instructions are required to retrieve the location of an object  from these dictionary-like objects. This slows down everything as the process is inefficient. V8 engine uses a different method known as `hidden classes` to find the addresses of properties of objects. Hidden classes are created at runtime. V8 attaches a hidden class to each object. Let's have a look on a simple function:
```
function Numbers(x,y){
    this.x = x;
    this.y = y;
}

var nums = new Numbers(5,20);
```
Once the new function is declared, a hidden class, let's say C0 will be created. Once the first statement `this.x = x` is hit, a new hidden class, let's say C1 will be created. C1 describes the location in memory where `x` can be found. A new property known as `class transition` is also added to C0. Class transition isn't a big deal. It means that whenever a new property `x` is added to a `Numbers` object, the hidden class should shift to C1. When the statement `this.y = y` is hit, a new hidden class C2 will be added and a new class transition will be added to C1 which says that if a property `y` is added to a `Numbers` object that already contains the property `x`, then the hidden class should change to C2 and the class is updated to C2. Thus, the main purpose of hidden classes is to optimize the access time when retrieving a property on an object.
Let's use a simple example to understand the memory optimization performed by the implementation of hidden classes:
```
function Student(name){
    this.name = name;
}

var name1 = new Student("alice");
var name2 = new Student("bob");
```
First of all the hidden class C0 will be created. When `new Student("alice")` is called, a new hidden class C1 will be created and a transition property will be added to it which states that if the property `name` gets added to an object of the `Student`, a transition to C1 should be done. Okay, so let us create a new object let's say `name2`. The class C0 already contains information about the transition. So, when the object `name2` adds the property `name` to it, no new hidden class will be created. Instead, the object `name2` simply changes the hidden class reference to C1. This prevents the creation of multiple hidden classes, thus improving memory management.

JavaScript engines use data structures known as `Inline Cache` to keep track of the addresses of the properties of objects. It relies upon the observation that repeated calls to the same function tends to occur on same types of objects. The Inline Cache tracks all the LOAD, STORE, and CALL events within a function, by maintaining a `Feedback Vector`. Feedback vector is nothing but an array used to track all the inline caches of a particular function. The next time when the same function is called, compiler can directly search for the type in cache.

createdReferences:
https://medium.com/@monica1109/how-does-web-browsers-work-c95ad628a509
https://dev.to/edisonpappi/how-javascript-engines-chrome-v8-works-50if
https://hacks.mozilla.org/2017/02/a-crash-course-in-just-in-time-jit-compilers/
https://www.geeksforgeeks.org/how-v8-compiles-javascript-code
https://richardartoul.github.io/jekyll/update/2015/04/26/hidden-classes.html