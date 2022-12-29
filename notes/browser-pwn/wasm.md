## Overview
Web-assembly is a low-level assembly like language with a compact binary format that runs with near-native performance. It is designed to run alongside JS, using the web assembly JS APIs.

## SharedArrayBuffer
A `SharedArrayBuffer` is used to represent a generic, fixed-length raw binary data buffer. It can be used to create views on shared memory. 

## WebAssembly JavaScript API

* `WebAssembly.Memory` object is a resizable `ArrayBuffer` or `SharedArrayBuffer` that holds raw bytes of memory accessed by a `WebAssembly.Instance`.

* The read-only `buffer` prototype property of the `WebAssembly.Memory` object returns the buffer contained in memory. Depending or not the memory was constructed with `shared:true`, the buffer is either an `ArrayBuffer` or `SharedArrayBuffer`.
