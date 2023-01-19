### Coroutines
Coroutines were introduced with C++20. A coroutine is a function that can suspend execution to be resumed later. They are stackless: they suspend execution by returning to the caller and the data that is required to resume execution is stored separately from the stack. A function is a coroutine if its definition contains any one of `co_await`(to suspend execution until resumed) , `co_yield`(to suspend execution returning a value) or `co_return`(to complete execution returning a value).

- A coroutine submits its result or exception through the `promise` object.
- Each coroutine contains the `coroutine handle`, which is manipulated from outside the coroutine. This is a non-owning handle used to resume execution of coroutine or to destroy the coroutine frame.
- Each coroutine contains a `coroutine state`, which is allocated on the heap. It stores the `promise` object, parameters (copied by value), some representation of the current suspension point, so that a resume knows where to continue.

When a coroutine begins execution, a `coroutine state` object is allocated on the heap using `operator new`, copies all the function parameters to the `coroutine state`, `by-value` parameters are copied and `by-reference` parameters remain references.