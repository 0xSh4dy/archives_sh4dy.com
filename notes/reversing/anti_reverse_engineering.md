# Anti Reverse Engineering


Anti reverse engineering refers to the techniques that make it difficult to reverse engineer a malware.
------------------------------------------------------------------------------------------------------
1. Anti Disassembly
Anti disassembly uses specially crafted code or data in a program to cause disassembly analysis tools to produce an incorrect program listing. Basically, two algorithms are used for disassembling a program: linear disassembly and flow-oriented disassembly. Linear disassembly algorithm disassembles one instruction at a time linearly until the end of the buffer. It fails to tell the difference between code and data. Flow-oriented disassemblers make assumptions and can make decisions in situations like conditional branching. IDA Pro uses a flow-oriented disassembler which examines each instruction and builds a list of locations to disassemble.

- Jump instructions with the same target
  This is produced by a combination of `jz` and `jnz` (two conditional jump instructions) that produces an unconditional jump.

------------------------------------------------------------------------------------------------------

2. Anti Debugging
It is used to find out whether a program is being debugged or not. Once a malware realizes that it is running in a debugger, it may alter its normal execution path or modify the code to cause a crash. The PEB contains a field known as `IsDebugged` which is set if a debugger is attached to the current process. Some famous methods for detecting debuggers are:
- Windows API functions such as `IsDebuggerPresent`, `CheckRemoteDebuggerPresent`, `NtQueryInformationProcess` and `OutputDebugString`. `NtQueryInformationProces` is a function that retrieves information about a given process. Using the value `ProcessDebugPort` (value 0x7) for the second parameter will tell if the process in question is currently being debugged. If not, zero will be returned.

- Instead of directly using the API functions mentioned above, the PEB structure can be analyzed manually for debugger detection. Various flags such as `BeingDebugged`, `ProcessHeap`(the first heap contains a header with fields used to tell the kernel whether the heap was created within a debugger using fields known as `ForceFlags` and `Flags`). If the value of the `NTGlobalFlag` field in the PEB structure is 0x70, the current process is being debugged.

- Debugging tools leave residue on the system. There's a registry key `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\DebugObjectRPCEnabled\AeDebug` or `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug` which specifies the debugger that activates when an application error occurs. By default, it is set to `Dr.Watson`.

- Using `FindWindow` with the first argument as the name of a debugger might be helpful in debugger detection.

- Every malware analyst performs operations like setting breakpoints, single step, etc. during dynamic analysis. When these operations are performed, the process code is modified. Let's have a deep dive into the behavior shown by debuggers and see how to interfere with the debugger functionality.

### Software Breakpoints
A software breakpoint is simply an asm instruction `INT3` which is interpreted by the CPU and triggers an interrupt calling the OS interrupt handler. The windows kernel converts this interrupt into an `EXCEPTION_DEBUG_EVENT` with an exception code of `EXCEPTION_BREAKPOINT`. The debugger writes the `INT3` instruction (0xcc) into the target process memory at the breakpoint location. The debugger also maintains a record of where the `INT3` opcodes have been inserted into the target memory and the original opcodes replaced by `INT3`. When the process is resumed, the debugger replaces the 0xcc opcode with the original byte, moves instruction pointer one byte back and then continues the execution. Since software breakpoints modify the process memory, they can be easily detected by using anti debugging techniques.
A malware can calculate a checksum on a code section and find out if it is change to detect software breakpoints. This technique can be evaded by using hardware breakpoints.

### Hardware Breakpoints
Hardware breakpoints are directly set in the CPU by adding the breakpoint address to CPU registers `DR0`,`DR1`, `DR2` or `DR3`: max four breakpoints. They do not rely on modification of target code, they can also be triggered on read or write using control flags in the `DR7` register. When a hardware breakpoint is triggered, an `EXCEPTION_DEBUG_EVENT`  is generated with the exception code of `EXCEPTION_SINGLE_STEP`. They are more stealthier than software breakpoints. The `DR7` controls the `DR0,DR1,DR2 and DR3` registers. The least significant 8 bits of DR7 uses a bitmask to enable or disable DR0 to DR3 registers as local or global breakpoints. 

```
Least significant 8 bits of DR7

_ _       _ _      _ _        _ _

DR3       DR2      DR1        DR0
```
If the LSB for the two bytes corresponding to a register is set, then it is enabled as a local breakpoint else as a global breakpoints. Global breakpoints are not present in windows.

The most significant 16 bits of DR7 are used to determine the type and size of breakpoints. The 16 bits are divided into four blocks of 4 bits, each for a DR0-3 register. The least significant two bits tell about the type of breakpoint i.e `00`:execute, `01`:write, `11`:read. The most significant two bits tell about the breakpoint size. `00` for 1 byte, `01` for two bytes, `10` for 8 bytes and `11` for 4 bytes. One way to detect hardware breakpoints is to query the values present in the registers mentioned above.

### Timing Checks
This is the most famous technique used by malwares to detect debuggers because processes run more slowly when being debugged. One way to do this is to record a timestamp, do some operations, take another timestamp and then compare them. If there is a lag, a debugger is certainly present. Another method is to take a timestamp before and after raising an exception. If a process is not being debugged, the exception will be handled really quickly. The `rdtsc` instruction (Read time-stamp counter, opcode 0x0f31) is used to perform a timing check. This instruction returns the count of the number of ticks since the last system reboot as a 64 bit value placed into EDX:EAX (higher 32 bits into the EDX and lower 32 bits into the EAX). This instruction is executed twice and the difference between the two readings is compared.
Instead of writing the code in asm, windows API functions such as `QueryPerformanceCounter` and `GetTickCount` can be used to perform an anti-debugging timing check. For example,
```
a = GetTickCount();
MaliciousFunction();
b = GetTickCount();
delta = b-a;
if(delta > 0x1a){
	// debugger present
}
else{
	debugger not present
}
```

### TLS Callbacks
Most debuggers start execution at the program's entry point mentioned in the PE header. A TLS callback can be used to execute code before the entry point and therefore execute secretly in a debugger. TLS stands for Thread Local Storage. TLS callbacks are subroutines executed before the entry point. In TLS, a data object is not an automatic stack variable, yet is local to each thread that runs the code. TLS allows each thread to maintain a different value for a variable declared using TLS. When TLS is implemented by an executable, the code will typically maintain a .tls section in the PE header. IDA easily detects TLS callbacks as the TLS callback functions are prefixed as `TlsCallback`. 

### Exceptions
Exceptions can be used for debugger detection. Most exception based detection relies on the fact that debuggers will trap the exception and not immediately pass to the process being debugged. the default setting on most debuggers is to trap exceptions.

### Inserting Interrupts
Inserting interrupts in the middle of a valid instruction sequence can be used to generate interrupts which can annoy the malware analyst. Inserting multiple `INT3` instructions (0xcc opcode) can trick the debugger (it would think that these are breakpoints). The 2-byte opcode sequence (0xcd03) can also be used to generate an `INT3` interrupt. It generates a `STATUS_BREAKPOINT` exception. `INT 0x2d` is used for kernel debuggers.


### Single Step Exceptions
This is a kind of trap generated by the CPU. It may aries when a breakpoint is hit, single step in debuggers,etc.

### In-Circuit Emulator (ICE, icebp, opcode 0xf1)
Executing this instruction generates a single step exception. If the program is being traced by single stepping, the debugger will think that it is the normal exception generated by single step and not execute a previously set exception handler.

### Trap flag
Generates a single step exception (int 0x01) after executing an instruction.
------------------------------------------------------------------------------------------------------

# Anti-Virtual Machine
Anti vm techniques are used to detect whether a malware is run inside a virtual machine or not. If a vm is detected, it might run differently or might not run.

The VMWare environment leaves many artifacts on the system. Malwares can use these artifacts, which are present in the filesystem, registry, and process listing, to detect VMware. MAC Addresses starting with some specific value may belong to Virtual box or VMware. Malwares may look for versions of hardware to detect virtualization. 
```
net start | findstr VMware, VMMouse, VMwareTray.exe
```

- Common x86 instructions used for VMware detection
	sidt,sgdt,sldt,smsw,str,in,cpuid

- The Red Pill Anti VM Technique
Red Pill is an Anti VM technique that executes the `sidt` instruction to grab the value of the `IDTR` register (Interrupt Descriptor Table, a table used to show the processor what interrupt service routine is called to handle an interrupt). IDTR stores a pointer to IDT. Execute the IDTR instruction and find out the value of the fifth byte returned. The value of the 5th byte in VMware is 0xff. This technique does not work on machines with multi core processors because each processor has an IDT assigned to it.

- The No Pill Anti VM Technique
This technique uses the `sgdt` and `sldt` instructions to detect VMware. Windows does not normally use the `LDT` structure, but VMware does. If the result of the `sldt` instruction is non-zero, the process is running in VMware. This can be fixed by disabling acceleration from VMware settings. No Pill solves this acceleration issue by using the `smsw` instruction if the `sldt` method fails. This methods involves inspecting the high-order bits returned by the `smsw` instruction.

- The I/O Communication Port
This is one of the most widely used Anti-VM techniques. Virtual machines use the i/o communication port for communication between the host operating system and the virtual machine (copy,paste,etc.). The communication port can be queried and compared with a magic number to detect a virtual machine. The x86 `in` instruction copies data from the I/O port specified by the source operand to a memory location specified by the destination operand. VMware monitors the use of the `in` instruction and captures the I/O destined for the communication channel port 0x5658 (VX).
```asm
mov eax,'VMXh'
mov ecx,0xa
mov dx, 'VX'
in eax,dx
```
The value 0xa means `get VMware version type` and 0x14 means `get the memory size`. 

`ScoopyNG` is a tool used for detecting VMware.

References: [Practical Malware Analysis](https://nostarch.com/malware)
https://1malware1.medium.com/anti-disassembly-techniques-e012338f2ae0
https://www.youtube.com/watch?v=PVnjYgoX_ck