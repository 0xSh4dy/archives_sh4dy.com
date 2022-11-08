---
layout: post
title: REvil Ransomware
subtitle: 
tags: [unpacking, x64dbg,ransomware,malware,ida]
comments: true
---

Smokeloader malware is primarily a loader, and its main objective is to download or load a stealthier or more effective malware into the system.

`Malware sample`:https://app.any.run/tasks/d72181bf-7bbc-46d6-9377-f420be98ad11/#
`MD5`: 5fc6f24d43bc7ca45a81d159291955d1
`SHA256`: fc20b03299b8ae91e72e104ee4f18e40125b2b061f1509d1c5b3f9fac3104934

Decompiling the `WinMain` function in IDA, we can see that the malware gets a handle to `kernel32.dll` and after that tries to receive the address of some exported function using `GetProcAddress`. However, the second argument of `GetProcAddress` isn't clear, probably it's encrypted.

--image--
Have a look on the following code snippet.
```c
for(k=0;k<dwSize;++k){
	*((_BYTE*)lpAddress+k) = *(_BYTE*)(dword_45CF0C+k+586102);
}
sub_403206((int*)&lpAddress,&dwSize);

flNewProtect = 64;
...
...

VirtualProtect(lpAddress, dwSize, flNewProtect, &flOldProtect);
sub_404B83((int)lpAddress, dwSize, (int)&unk_448000);
...
...
lpAddress = (char *)lpAddress + 3395;
return ((int (*)(void))lpAddress)();
```
The malware allocates memory using `LocalAlloc`, loads some data (shellcode) into it, calls `VirtualProtect` with the third argument as 0x40 (PAGE_EXECUTE_READWRITE) and then executes the shellcode.
Well, extracting the shellcode using static analysis is pretty difficult, so I'll use dynamic analysis to dump the shellcode. Let's load the executable in x32dbg and set breakpoints at `VirtualProtect` and the address where the shellcode is executed (`((int (*)(void))lpAddress)()`). 

--image--


Examine the `ESP` when the first breakpoint is hit (`VirtualProtect`). Here, `0x81CF48` is the address of the shellcode and `0xfbd0` its length. 
```
00401BC4 | A3 08CF4500              | mov dword ptr ds:[45CF08],eax                            |
00401BC9 | FF15 08CF4500            | call dword ptr ds:[45CF08]                               |
```
This says that a pointer to the shellcode buffer is stored in the `eax` register. Follow that value in dump and run the command `savedata "new_dump.bin",0x81dc8b,0xfbd0` to dump `0xfbd0` bytes starting from `0x81dc8b` into a file `new_dump.bin`. Now, we can load `new_dump.bin` in IDA in order to analyse the shellcode.

---image---

Interesting, isn't it?

Anyone with a basic knowledge of shellcoding in windows can figure out that this shellcode uses the `PEB` traversal technique for finding functions. 
```py
seg000:00000042 sub_42          proc near               ; CODE XREF: sub_10+13↑p
seg000:00000042                                         ; sub_10+25↑p
seg000:00000042
seg000:00000042 arg_0           = dword ptr  8
seg000:00000042 arg_4           = dword ptr  0Ch
seg000:00000042
seg000:00000042                 push    ebp
seg000:00000043                 mov     ebp, esp
seg000:00000045                 push    ebx
seg000:00000046                 push    esi
seg000:00000047                 push    edi
seg000:00000048                 push    ecx
seg000:00000049                 push    dword ptr fs:loc_30
seg000:00000050                 pop     eax
seg000:00000051                 mov     eax, [eax+0Ch]
seg000:00000054                 mov     ecx, [eax+0Ch]
```



