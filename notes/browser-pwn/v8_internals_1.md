---
layout: page
title: v8 Internals
comments: true
tags: [browser, v8, pwn]
---

### Isolate

Isolate represents an isolated instance of the V8 engine. v8 isolates have completely separate states. It includes heap manager, garbage collector, etc. Only one thread may access a given isolate at a time, but different threads may access different isolates simultaneously. Objects from one isolate must not be used in other isolates.

### Context

A context is nothing but an execution environment that allows separate JavaScript applications to run in the same isolate. You must explicitly specify the context in which you want to run any JS code. The motivation for using contexts in v8 was so that each window and iframe in a browser can have its own fresh JavaScript environment.

### Templates

A template is a blueprint for JS objects and functions in a context. Templates can be used for wrapping C++ data structures and functions within JavaScript objects so that they can be manipulated using JS.

#### Function Templates

A function template is the blueprint for a single function. A JavaScript instance of the template can be created by calling the template's `GetFunction` method from within the context in which you want to instantiate the JavaScript function. A C++ callback can also be associated with a function template which is called when the JavaScript function instance is invoked.

#### Object Templates

Each function template has an associated object template. This is used to configure objects created with this function as their constructor. You can associate two types of object callbacks with object templates: accessor callbacks and interceptor callbacks.

- Accessor callbacks are invoked when a specific object property is accessed by a script.
- Interceptor callbacks are invoked when any object property is accessed by a script.

#### Accessors

An accessor is a C++ callback that calculates and returns a value when an object property is accessed by a JS script. Accessors are configured through object templates, using the `SetAccessor` method. This method takes the name of the property with which it is associated and two callbacks to run when a script attempts to read or write the property.

#### Interceptors

A C++ callbacks invoked when any object property is accessed by a script. `Named property interceptors` are called when accessing properties with string names, for example `document.formName.elementName`. `Indexed property interceptors` are called when accessing indexed properties such as `document.forms.elements[0]`

### Prototype

Each object in JavaScript has a builtin property called prototype which is itself an object. A prototype has its own prototype, creating a prototype chain. The chain ends when we reach a prototype that has `null` for its own prototype. If a property cannot be found when accessing an object, its prototype is searched for the property. If the property still can't be found, then the prototype's prototype is searched, and so on until either the property is found, or the end of the prototype chain is reached, which returns `undefined`.

Each `FunctionTemplate` has a `PrototypeTemplate` method which gives a template for the function's prototype. You can set properties, and associate C++ functions with those properties, on a prototype template which will then be present on all instances of the corresponding `FunctionTemplate`.

```cpp
v8::Local<v8::FunctionTemplate> biketemplate = v8::FunctionTemplate::New(isolate);
biketemplate->PrototypeTemplate().Set(
    v8::String::NewFromUtf8(isolate, "wheels"),
    v8::FunctionTemplate::New(isolate, MyWheelsMethodCallback)->GetFunction()
);
```

This causes all instance of biketemplate to have a wheels method in their prototype chain which, when called, causes the C++ function `MyWheelsMethodCallback` to be called.

### Address

```c
typedef uintptr_t Address;
```

### TaggedImpl

A TaggedImpl is a base class for Object (which is either a Smi or a strong reference to a HeapObject) and MaybeObject (which is either a Smi, a strong reference to a HeapObject, a cleared weak reference or a weak reference to a HeapObject).

### Tagged Pointer
A tagged pointer is a pointer with additional data stored in it which may be some bits used for specific purposes such as reference count.

### Object
This class extends `TaggedImpl`. An Object can be created using the default constructor, or by passing in an Address that will delegate to `TaggedImpl` constructors. Object itself does not have any members (apart from ptr_ which is inherited from TaggedImpl).

### ObjectSlot

An ObjectSlot instance describes a slot holding a tagged pointer (smi or strong heap object). `address()` gives the address of the slot. The slot's contents can be read and written using operator\* and store().

### Maybe

Maybe is like an optional which can either hold a value or nothing.

```cpp
template <class T>
class Maybe {
 public:
  V8_INLINE bool IsNothing() const { return !has_value_; }
  V8_INLINE bool IsJust() const { return has_value_; }
  ...

 private:
  bool has_value_;
  T value_;
}
```

The usage of Maybe is where API calls may fail and returning nothing is a way of signaling this.

### MaybeLocal

```cpp
template <class T>
class MaybeLocal {
 public:
  V8_INLINE MaybeLocal() : val_(nullptr) {}
  V8_INLINE Local<T> ToLocalChecked();
  V8_INLINE bool IsEmpty() const { return val_ == nullptr; }
  template <class S>
  V8_WARN_UNUSED_RESULT V8_INLINE bool ToLocal(Local<S>* out) const {
    out->val_ = IsEmpty() ? nullptr : this->val_;
    return !IsEmpty();
  }

 private:
  T* val_;
```
`MaybeLocal<T>` is used when an API returns either a `T` or throws an error

### Data
It is the superclass of values (all objects that can exist in the V8 heap) and API object templates.
<img src=''>

### Value
It is a subclass of `Data` and adds a number of methods that check if a Value is of a certain type, like `IsUndefined`, `IsNull`, `IsNumber`, etc.

### Handle
An object reference managed by the v8 garbage collector. All objects returned from v8 have to be tracked by the garbage collector so that it knows if the objects are still alive. Because the garbage collector may move objects, it is unsafe to point directly to an object. Instead, all objects are stored in handles which are known by the garbage collector and updated whenever an object moves. Handles should always be passed by value and they should never be allocated on the heap. There are two types of handles: `Local` handles and `Persistent` handles. 
```cpp
template<class T> class v8::Handle< T >
```

#### Local Handles
A light-weight stack allocated object handle. All operations that return objects from v8 return them in local handles. They are created within `HandleScopes`, and all local handles allocated within a handle scope are destroyed when the handle scope is destroyed. 
```cpp
template<class T>class v8::Local< T >
```

### Persistent Handles
An object reference that is independent of any handle scope. Where a local handle only lives as long as the HandleScope in which it was allocated, a PersistentHandle remains valid until it is explicitly disposed.They provide a reference to a heap allocated JS object. Persistent handles should be used when a reference to an object is needed for more than one function call, or when handle lifetime does not correspond to C++ scopes.
```
template<class T>class v8::Persistent< T >
```

### HandleScope
A stack-allocated class that governs a number of local handles. After a `HandleScope` has been created, all local handles will be allocated within that handle scope until either the handle scope is deleted or another handle scope is created. If there is already a handle scope and a new one is created, all allocations will take place in the new handle scope until it is deleted. After that, new handles will be created in the original handle scope. After the HandleScope of a local handle has been deleted the garbage collector will no longer track the object stored in the handle and may deallocate it.

### Implementation of the JS standard library
There are various ways to implement the JS standard library such as the C++ API, assembly and JavaScript itself. But each of these methods has some bad effect.

- Using the C++ API
  C++ is a high level language, it has compiler optimizations, it is fast which makes it a good choice for implementing the standard library. However, C++ is not good at garbage collection and v8 is a garbage collected engine, the objects in v8 are garbage collected. Objects on the v8 heap can move anytime which must be handled with care which makes C++ a bad choice for it. Making transition from JS to C++, checking different calling conventions,etc. is expensive. This shouldn't be done for functions that should be called quickly.

- Using assembly
  Obviously, assembly is fast but it is too prone to errors and bugs.

- Using JavaScript
  JavaScript is easily readable, compatible with v8's garbage collector. However, JS semantics can be exploited to get memory leaks. Exposing the VM internal structs is necessary to increase the execution speed. But, exposing internal structures to a user controlled language like JS would make the browser vulnerable to memory leaks.

- Using v8's optimizing compiler
  The optimizing compiler can access the internal structs without exposing them to external users, provides low-level control, provides a good control on semantics. In order to use the optimizing compiler for implementing the library, the developers will need to write code in IR which is a low-level language and difficult to read and write.

- CodeStubAssembler(CSA)
  It is a C++ API used to generate IR for v8's optimizing compiler which is then compiled into machine code for the target machine. It is based on the backend of Turbofan, v8's optimizing compiler.

- Torque
  It is a V8-specific domain-specific language built on the top of CSA. It is statically typed, garbage collected, compatible with JavaScript and easily readable. The Torque compiler doesn't create machine code directly, but generates C++ code that calls V8's existing CodeStubAssembler interface.

### Torque Macros

Macros are just like inline functions. They are used to remove the effect of call overhead.

### Transient Types

In v8,heap objects can change layout at runtime. To express object layouts that are subject to change or other temporary assumptions in the type system, torque supports the concept of transient types.

References:

[https://v8.github.io/api/head/index.html](https://v8.github.io/api/head/index.html)
[https://youtu.be/YcroNGHHqtU](https://youtu.be/YcroNGHHqtU)
[https://github.com/danbev/learning-v8](https://github.com/danbev/learning-v8)
