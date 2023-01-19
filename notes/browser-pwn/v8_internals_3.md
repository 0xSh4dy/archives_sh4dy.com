### BackingStore
A backing store is a device for secondary storage of data that typically has greater capacity than the primary store but is slower to access. The BackingStore data structure stores all the low-level details about the backing store of an array buffer or wasm memory, including its base address and length, whether it is shared, provided by the embedder, has guard regions, etc. Backing stores can also wrap embedder-allocated memory. In this case, they do not own the memory, and upon destruction, they do not deallocate it. 

### ArrayBuffer
The ArrayBuffer object is used to represent a generic, fixed-length raw binary data buffer. In simple words, it is a bytearray. Backing stores of ArrayBuffers are allocated outside the v8 heap using `ArrayBuffer::Allocator` provided by the embedder. The v8 garbage collector cannot simply collect unreachable ArrayBuffer objects. Instead it has to invoke the external deallocator for their backing stores. It can be done by iterating a hash table that maps an ArrayBuffer object to the deallocator of its backing store. The hash table is partitioned by page resulting in per-page hash tables. Using hash tables may cause overhead. Checkout the [following](https://docs.google.com/document/d/1-ZrLdlFX1nXT3z-FAgLbKal1gI8Auiaya_My-a0UJ28/edit#) document to see how this problem of hash table overhead is solved .

### PartitionAlloc
PartitionAlloc is a memory allocator optimized for space efficiency, allocation latency and security. For more information, checkout [this post](https://chromium.googlesource.com/chromium/src/+/HEAD/base/allocator/partition_allocator/PartitionAlloc.md).

### V8 Heap
JS values in v8 are represented as objects and allocated on the V8 heap, no matter if they are objects, numbers, arrays and strings. The v8 heap consists of multiple mmapped sections of memory that are usually the lowest memory mappings in the program. The upper 32 bits of the heap is always the same for a particular run. Since, the upper 32 bits of each pointer are the same, it is a wastage of memory to store them multiple times. The v8 team decided to store only the lower 32 bits in the pointer. The upper 32 bits are stored as a base address in a location called the `root register`. To get the actual address, the value stored in the `root register` (R13) can be added to the 32 bit address. This restricts the size of the v8 heap to 4GB. It becomes a problem for stuff like Node.js that require larger heaps. Because of this, pointer compression is disabled for node.js



References
[https://blog.infosectcbr.com.au/2020/02/pointer-compression-in-v8.html]
[https://docs.google.com/document/d/1-ZrLdlFX1nXT3z-FAgLbKal1gI8Auiaya_My-a0UJ28/edit#]
[https://chromium.googlesource.com/chromium/src/+/HEAD/base/allocator/partition_allocator/PartitionAlloc.md]