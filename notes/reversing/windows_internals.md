## kernel32.dll
This is a 32 bit dynamic link library used in windows to handle memory management, input/output operations, interrupts, synchronization, and process creation. It also exposes most of the win32 base APIs to applications. It is loaded into protected memory when the system boots and runs as a background process. kernel32.dll is always loaded into the same address for all the processes. 

## ntdll.dll
This dll exports the native API (the syscall interface). Most of the symbols exported by this dll are prefixed as `Nt`. Native APIs are used to implement many of the kernel APIs exported by kernel32.dll. The native API is used by the usermode components of the operating system that must run without support from Win32 or other API subsystems. ntdll.dll is always loaded into the same address for all the processes.

---------------------------------------------------------------------------------------------------
## PE File
The Portable Executable (PE) is a file format for executables, object code, DLLs and others used in 32 and 64 bit windows operating systems. The PE format is a data structure that encapsulates the information necessary for the Windows OS loader to manage the wrapped executable code. 

### DOS Header
The DOS header contains relocation information, which allows multiple segments to be loaded at arbitrary memory addresses. It occupies the first 64 bytes of an executable. The field `e_magic` represents the magic number which is used to identify an MS-DOS compatible file. Its value is 0x5A4D (which means `MZ`, when converted to characters). 
```c
typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
    USHORT e_magic;         // Magic number
    USHORT e_cblp;          // Bytes on last page of file
    USHORT e_cp;            // Pages in file
    USHORT e_crlc;          // Relocations
    USHORT e_cparhdr;       // Size of header in paragraphs
    USHORT e_minalloc;      // Minimum extra paragraphs needed
    USHORT e_maxalloc;      // Maximum extra paragraphs needed
    USHORT e_ss;            // Initial (relative) SS value
    USHORT e_sp;            // Initial SP value
    USHORT e_csum;          // Checksum
    USHORT e_ip;            // Initial IP value
    USHORT e_cs;            // Initial (relative) CS value
    USHORT e_lfarlc;        // File address of relocation table
    USHORT e_ovno;          // Overlay number
    USHORT e_res[4];        // Reserved words
    USHORT e_oemid;         // OEM identifier (for e_oeminfo)
    USHORT e_oeminfo;       // OEM information; e_oemid specific
    USHORT e_res2[10];      // Reserved words
    LONG   e_lfanew;        // File address of new exe header
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
```

### DOS Stub
DOS Stub is an MS-DOS compatible executable which is invoked if the program is run in MS-DOS. This prints the error message `This program cannot be run in DOS mode` when the program is run in DOS. 

### NT Headers
NT header is a structure defined in `winnt.h` which contains three main parts: the PE signature, the file header and the optional header.
```c
typedef struct _IMAGE_NT_HEADERS {
  DWORD                   Signature;
  IMAGE_FILE_HEADER       FileHeader;
  IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32
```

`Signature`: A four-byte("PE\0\0") signature that identifies a file as a PE image.
<br>
`FileHeader`: An `IMAGE_FILE_HEADER` structure that specifies the file header. It represents the COFF header format. COFF (Common Object File Format ) is a format for executables, object code, and shared libraries used on Unix based systems. 
```c
typedef struct _IMAGE_FILE_HEADER {
  WORD  Machine;
  WORD  NumberOfSections;
  DWORD TimeDateStamp;
  DWORD PointerToSymbolTable;
  DWORD NumberOfSymbols;
  WORD  SizeOfOptionalHeader;
  WORD  Characteristics;
} IMAGE_FILE_
```
<br>
`OptionalHeader`: An `IMAGE_OPTIONAL_HEADER` structure that specifies the optional file header. Object files do not have this header, however it is required by image files (such as .exe). This header provides important information to the OS loader. 

```c
typedef struct _IMAGE_OPTIONAL_HEADER
{
     WORD Magic;
     UCHAR MajorLinkerVersion;
     UCHAR MinorLinkerVersion;
     ULONG SizeOfCode;
     ULONG SizeOfInitializedData;
     ULONG SizeOfUninitializedData;
     ULONG AddressOfEntryPoint;
     ULONG BaseOfCode;
     ULONG BaseOfData;
     ULONG ImageBase;
     ULONG SectionAlignment;
     ULONG FileAlignment;
     WORD MajorOperatingSystemVersion;
     WORD MinorOperatingSystemVersion;
     WORD MajorImageVersion;
     WORD MinorImageVersion;
     WORD MajorSubsystemVersion;
     WORD MinorSubsystemVersion;
     ULONG Win32VersionValue;
     ULONG SizeOfImage;
     ULONG SizeOfHeaders;
     ULONG CheckSum;
     WORD Subsystem;
     WORD DllCharacteristics;
     ULONG SizeOfStackReserve;
     ULONG SizeOfStackCommit;
     ULONG SizeOfHeapReserve;
     ULONG SizeOfHeapCommit;
     ULONG LoaderFlags;
     ULONG NumberOfRvaAndSizes;
     IMAGE_DATA_DIRECTORY DataDirectory[16];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;
```

### Section Table
The Section Table is an array of section headers. Sections are the containers of the actual data present in an executable. Section headers store information about various sections of a PE file. Section header is defined as `_IMAGE_SECTION_HEADER` in `winnt.h`
```c
typedef struct _IMAGE_SECTION_HEADER {
    BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
            DWORD   PhysicalAddress;
            DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
```
Some of the sections present in a PE file are:
`.text`: contains the executable code of the program
<br>
`.data`: contains the initialized data
<br>
`.bss`: contains uninitialized data
<br>
`.rdata`: contains read-only initialized data
<br>
`.edata`: contains the export tables
<br>
`.idata`: contains the import tables
<br>
`.reloc`: contains image relocation information
<br>
`.rsrc`: contains resources used by the program such as images, icons, embedded binaries etc.
<br>
`.tls`(thread local storage): provides storage for every executing thread of the program.


## Data Directory
```c
typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD   VirtualAddress;
    DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
```
A data directory is nothing but a piece of data present in a PE file. Data directories store useful information needed by the loader. Some examples of data-directories are the Import Directory, export directory, resource directory, etc. They contain RVAs to various tables such as exports, imports , resources etc.
```c
#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor
```

## Import Directory Table
It is a data directory present at the beginning of the `.idata` section. It does not have a fixed size. It contains entries for every DLL which is loaded by the executable. It is made up of an array of `IMAGE_IMPORT_DESCRIPTOR` structures, where each structure is for a DLL.
```c
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    union {
        DWORD   Characteristics;
        DWORD   OriginalFirstThunk;
    } DUMMYUNIONNAME;
    DWORD   TimeDateStamp;
    DWORD   ForwarderChain;
    DWORD   Name;
    DWORD   FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;
typedef IMAGE_IMPORT_DESCRIPTOR UNALIGNED *PIMAGE_IMPORT_DESCRIPTOR;
```
`OriginalFirstThunk` stores the RVA of the Import Lookup Table (ILT).

## Bound Imports
The import table stores fixed addresses of the imported functions. Bound addresses are calculated and written to the import table by the linker at compile time. The Bound Import Data Directory stores information about bound imports.

## Delay-Load Import Tables
The loading of a DLL is delayed until the first call to that DLL. 

## Import Lookup Table (or Import Name Table)
The Import Lookup Table is an array of 32 bit numbers for PE32 executables and 64 bit numbers for PE32+ executables. It tells the loader which functions are needed from the imported DLL. Each entry uses the bit-field format. The 

`Bit 31/63` -> This bit specifies whether to import the function by name or by ordinal. 
<br>
`Bit 15-0` -> It is a 16 bit ordinal number which is only set if bit 31/63 (for PE32 and PE32+ respectively) is set. Bits 30-15 or 62-15 must be zero.
<br>
`Bit 30-0` -> A 31 bit RVA of a hint/name table entry.

The hint-name table looks as follows:
```c
typedef struct _IMAGE_IMPORT_BY_NAME {
    WORD    Hint;
    CHAR   Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;
```
Hint is a number used as a key to lookup a function whose name is defined in the `Name` field


## Import Address Table (IAT)
The Import Address Table (IAT) is comprised of function pointers, and is used to get the addresses of functions when DLLs are loaded. The entries of IAT are overwritten at runtime (similar to lazy binding in ELFs). 

## Relocations and the Relocation Table
The relocation table is a lookup table that lists all parts of the PE file that need patching when the file is loaded at a non-default base address. When a program is compiled, the compiler assumes that the executable is going to be loaded at a certain base address. That address is saved in `IMAGE_OPTIONAL_HEADER.ImageBase`. Some calculations are made to calculate other addresses and then they are hardcoded into the executable based on the base address. Many times the executable will be loaded at some different base address. So, the relocation table stores information about all the hardcoded values that should be fixed if the executable is loaded at a different base address. 

-----------------------------------------------------------------------------------------------------


## Virtual Address (VA)
A virtual address is a pointer to a memory area that an operating system allows a process to use.

## Relative Virtual Address (RVA)
Address of some item in memory minus the base address of the image.

---------------------------------------------------------------------------------------------------




---------------------------------------------------------------------------------------------------
## Thread Environment Block (TEB)
The Thread Environment Block is a structure that stores information about the currently running thread. 
```c
typedef struct _TEB {
  PVOID Reserved1[12];
  PPEB  ProcessEnvironmentBlock;
  PVOID Reserved2[399];
  BYTE  Reserved3[1952];
  PVOID TlsSlots[64];
  BYTE  Reserved4[8];
  PVOID Reserved5[26];
  PVOID ReservedForOle;
  PVOID Reserved6[4];
  PVOID TlsExpansionSlots;
} TEB, *PTEB;
```

## Process Environment Block (PEB)
PEB is a data structure that stores information about the current process such as the command line used to invoke the process, presence of debugger, modules loaded into memory, etc. 
```c
typedef struct _PEB {
  BYTE                          Reserved1[2];
  BYTE                          BeingDebugged;
  BYTE                          Reserved2[1];
  PVOID                         Reserved3[2];
  PPEB_LDR_DATA                 Ldr;
  PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
  PVOID                         Reserved4[3];
  PVOID                         AtlThunkSListPtr;
  PVOID                         Reserved5;
  ULONG                         Reserved6;
  PVOID                         Reserved7;
  ULONG                         Reserved8;
  ULONG                         AtlThunkSListPtr32;
  PVOID                         Reserved9[45];
  BYTE                          Reserved10[96];
  PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
  BYTE                          Reserved11[128];
  PVOID                         Reserved12[1];
  ULONG                         SessionId;
} PEB, *PPEB;
```

## PPEB_LDR_DATA (Ldr)
This structure stores information about the user-mode modules that are loaded in a process.
```c
typedef struct _PEB_LDR_DATA {
  BYTE       Reserved1[8];
  PVOID      Reserved2[3];
  LIST_ENTRY InMemoryOrderModuleList;
} 
```

## InMemoryOrderModuleList
This is the head of a doubly linked list that contains the loaded modules for a process. Each item in the list is a pointer to the `LDR_DATA_TABLE_ENTRY` structure.
```c
typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *RESTRICTED_POINTER PRLIST_ENTRY;
```

```c
typedef struct _LDR_DATA_TABLE_ENTRY {
    PVOID Reserved1[2];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[2];
    PVOID DllBase;
    PVOID EntryPoint;
    PVOID Reserved3;
    UNICODE_STRING FullDllName;
    BYTE Reserved4[8];
    PVOID Reserved5[3];
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    };
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

```

## Finding the base address of kernel32.dll
Most of the malwares used the PEB traversal technique to find the base address of kernel32.dll. We can find offsets of different items in a struct using Windbg. Let's find out the offset of `ProcessEnvironmentBlock` in the `TEB` structure.
```c
0:007> dt _teb
ntdll!_TEB
   +0x000 NtTib            : _NT_TIB
   +0x01c EnvironmentPointer : Ptr32 Void
   +0x020 ClientId         : _CLIENT_ID
   +0x028 ActiveRpcHandle  : Ptr32 Void
   +0x02c ThreadLocalStoragePointer : Ptr32 Void
   +0x030 ProcessEnvironmentBlock : Ptr32 _PEB
..........
```
This shows that the ProcessEnvironmentBlock is present at the offset 0x30. `Ldr` is present at the offset `0xc` in `PEB`. `InMemoryOrderModuleList` is stored at the offset 0x14 in `_PEB_LDR_DATA`. `InMemoryOrderModuleList` is a doubly linked list where each list item points to a `LDR_DATA_TABLE_ENTRY` structure. 

```c
ntdll!_LDR_DATA_TABLE_ENTRY
   +0x000 InLoadOrderLinks : _LIST_ENTRY
   +0x008 InMemoryOrderLinks : _LIST_ENTRY
   +0x010 InInitializationOrderLinks : _LIST_ENTRY
   +0x018 DllBase          : Ptr32 Void
   +0x01c EntryPoint       : Ptr32 Void
   +0x020 SizeOfImage      : Uint4B
   +0x024 FullDllName      : _UNICODE_STRING
   +0x02c BaseDllName      : _UNICODE_STRING
   +0x034 FlagGroup        : [4] UChar
   ......
```
`DllBase` (present at offset 0x18) stores the base address of the module. 

```r
0:007> !peb
PEB at 00205000
    InheritedAddressSpace:    No
    ReadImageFileExecOptions: No
    BeingDebugged:            Yes
    ImageBaseAddress:         009f0000
    NtGlobalFlag:             0
    NtGlobalFlag2:            0
    Ldr                       77535d80
    Ldr.Initialized:          Yes
    Ldr.InInitializationOrderModuleList: 00704b38 . 00729e18
    Ldr.InLoadOrderModuleList:           00704c10 . 0072af38
    Ldr.InMemoryOrderModuleList:         00704c18 . 0072af40
            Base TimeStamp                     Module
          9f0000 6255d9bd Apr 13 01:27:49 2022 C:\Users\raksh\AppData\Local\Discord\app-1.0.9006\Discord.exe
        77410000 10040106 Jul 08 03:47:42 1978 C:\WINDOWS\SYSTEM32\ntdll.dll
        770e0000 20566761 Mar 12 13:14:33 1987 C:\WINDOWS\System32\KERNEL32.DLL

```
kernel32.dll base can be found by finding the value of the `DllBase` field of the third node in the `InMemoryOrderModuleList` linked list. Once we have the `kernel32.dll` base, we can loop through all the exported functions of the module to find the function we're interested in. A number of steps need to be carried out to do this. 

- Windows generally uses the Hungarian notation for API function identifiers. 

## Handles
Handles are items that have been opened or created in the OS, such as a window, process, module, file, menu, etc.

## File Mappings
File mappings allow a file to be loaded into memory and manipulated easily. The `CreateFileMapping` function loads a file from disk into memory. `MapViewOfFile` returns a pointer to the base address of the mapping, which can be used to access the file in memory. This functionality is used by launchers, loaders and injectors to read and modify PE files.


## Some useful Windbg commands
```
dt _teb
dt _peb
dt _PEB_LDR_DATA
!peb
```




## References:
https://0xrick.github.io/win-internals/pe1/
https://www.ired.team/offensive-security/code-injection-process-injection/finding-kernel32-base-and-function-addresses-in-shellcode
http://practicalmalwareanalysis.com/
https://en.wikipedia.org/wiki/Hungarian_notation
https://gist.github.com/404NetworkError/a81591849f5b6b5fe09f517efc189c1d
https://www.malwarebytes.com/blog/news/2015/07/introduction-to-alternate-data-streams