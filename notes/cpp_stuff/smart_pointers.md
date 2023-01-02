---
layout: page
title: Smart Pointers
tags: [cpp]
comments: true
---

Smart pointers are wrappers around regular pointers . They are used to make sure that an object is deleted if it is no longer used. There are four types of smart pointers in C++ namely `std::auto_ptr`,`std::unique_ptr`,`std::shared_ptr` and `std::weak_ptr`

### std::unique_ptr

```cpp
template<class T,class Deleter = std::default_delete<T>> class unique_ptr;

template <class T,class Deleter> class unique_ptr<T[], Deleter>;
```

A smart pointer that owns and manages another object through a pointer and disposes it when the `std::unique_ptr` goes out of scope. The object is disposed of, using the associated deleter when either the managing unique_ptr object is destroyed or the managing unique_ptr object is assigned another pointer via `operator=` or `reset()`. `std::unique_ptr` is used for `exclusive-ownership resource management`. The object is deleted using a user-supplied deleter by calling `get_deleter()(ptr)`. The default deleter user the `delete` operator, which destroys the object and deallocates the memory.

Note: copying a unique_ptr is not allowed because copying would create std::two unique_ptrs to the same resource, each thinking it owned (and should therefore destroy) the resource which may cause double free. However, the `move` operation can be performed on std::unique_ptrs which transfers ownership from the source pointer to the destination pointer.
The default size of a std::unique_ptr is same as the size of pointers (4 or 8 bytes depending on the architecture). Using custom deleters may increase the size. `std::unique_ptr`can  be easily converted into `std::shared_ptr`.

### std::shared_ptr
```cpp
template< class T > class shared_ptr;
```
A smart pointer that retains shared ownership of an object through a pointer. Thus, it is used for `shared-ownership resource management`. Several shared_ptrs can own the same object. The object is destroyed and its memory is deallocated if either the last remaining `std::shared_ptr` owning the object is destroyed or the last remaining `shared_ptr` owning the object is assigned another pointer via `operator=` or `reset()`.
A `std::shared_ptr` can tell whether it's the last one pointing to a resource by consulting the resource's `reference count`, a value associated with the resource that keeps track of how many `std::shared_ptr`s point to it. `std::shared_ptr` constructors increment it, destructors decrement it, and copy assignment operators do both. If `sp1` and `sp2` are `std::shared_ptr`s to different objects, then the assignment `sp1=sp2` modifies `sp1` such that it points to the object pointed to by `sp2`. This decrements the reference count for the object originally pointed to by `sp1` and increments it for the object pointed to by `sp2`. If a `std::shared_ptr` sees a reference count of zero after performing a decrement, it destroys the object.

- The size of `std::shared_ptr` is twice the size of a raw pointer, because they contain a raw pointer to the resource as well as a raw pointer to the reference count.
- Reference count is associated with the object being pointed to but the objects know nothing about it, and have no extra space to store the reference count. Memory for reference count must be avoided dynamically to avoid memory corruption bugs.
- Moving `std::shared_ptr` is faster than copying because copying increments the reference count but moving doesn't.
- Specifying a custom deleter doesn't change the size of a `std::shared_ptr` object.
- Reference count is a part of a larger data structure known as a control block. A `std::shared` ptr object stores a pointer to the control block. Thus, the other pointer in an `std::shared_ptr` object doesn't actually point to the reference count, but points to the `control block`.

#### Control Block
A control block is a dynamically allocated object that holds either a pointer to the managed object or the managed object itself, the deleter, the allocator , number of std::shared_ptrs to the object (reference count) and the number of std::weak_ptrs to the object (weak count). 

- An object's control block is setup by the function creating the first `std::shared_ptr` to the object. For example, `std::make_shared` creates a control block
- A control block is created when a `std::shared_ptr` is constructed from a unique-ownership pointer
- When a `std::shared_ptr` object's constructor is called with a raw pointer, it creates a control block.
- Avoid passing raw pointers to a `std::shared_ptr` constructor. Constucting more than one `std::shared_ptr` objects from the same raw pointer will create multiple control blocks for the same object which means that the object will be destroyed multiple times.

### std::weak_ptr
```cpp
template< class T > class weak_ptr;
```
A smart pointer that holds a non-owning (weak) reference to an object that is managed by `std::shared_ptr`. They can neither be dereferenced, nor tested for nullness. They are managed by `std::shared_ptr`s. They point to the same place as the `std::shared_ptr` s initialize them, but do not affect the reference count of the object they point to. `std::weak_ptr`s that dangle are said to be expired. `std::weak_ptr` objects are of the same size as `std::shared_ptr` objects and make use of same control blocks as `std::shared_ptr`s. 
Consider a data structure with objects A,B and C in it, where A and C share ownership of B and therefore hold `std::shared_ptr`s to it. Suppose it would be useful to have a pointer from B back to A. There are three choices for this pointer: raw pointer, std::shared_ptr and std::weak_ptr. 
- In case of raw pointer, if A is destroyed, C will still point to B and B will still contain a pointer to A that will dangle.  B will not be able to detect the dangling pointer and may inadvertently dereference it.
- In case of `std::shared_ptr`, A and B contain `std::shared_ptr`s to each other. The resulting cycle will prevent both A and B from being destroyed because the reference count will never be zero.
- If A is destroyed, B's pointer back to it will dangle, but B will be able to detect that because it's a `std::weak_ptr`. Also, B's pointer will not affect A's reference count.

### Using new vs std::make_shared / std::make_unique
Consider two examples:
```cpp
std::shared_ptr<int> p(new int);
auto p = std::make_shared<int>(); 
```
When creating a `std::shared_ptr` using constructor (example 1),two allocations are made, one for the object and one for the control block which causes overhead. Using `std::make_shared` offers a performance advantage by doing a single allocation from the heap.


References

[https://en.cppreference.com/](https://en.cppreference.com/)
