---
layout: page
title: v8 Internals
comments: true
tags: [browser,v8,pwn]
---

### Heap Object
A pointer that points to memory in the managed heap. It's a superclass for everything allocated in the heap. If the last bit is set to 1, before using the pointer, this bit needs to be cleared.

### Map
Every object that is stored on the v8 heap has a map that describes the structure of the object being stored. A HeapObject contains a pointer to a map, or rather has a function that returns a pointer to map.
```cpp
class Map:public HeapObject{
    ...
}
```

### Threads
V8 is single threaded (the execution of functions of the stack) but there are supporting threads used for garbage collection and profiling.

### V8_EXPORT
```c
#if V8_HAS_ATTRIBUTE_VISIBILITY && defined(V8_SHARED)
# ifdef BUILDING_V8_SHARED
#  define V8_EXPORT __attribute__ ((visibility("default")))
# else
#  define V8_EXPORT
# endif
```
Functions with default visibility have a global scope and can be called from other shared objects. Functions with hidden visibility have a local scope and cannot be called from other shared objects. When compiling with `-fvisibility=hidden`, GCC assumes every declaration not marked with a visibility attribute has a hidden visibility.

References:
[https://github.com/danbev/learning-v8](https://github.com/danbev/learning-v8)