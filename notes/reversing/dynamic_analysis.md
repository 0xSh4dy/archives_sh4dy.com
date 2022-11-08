Dynamic analysis is performed by running the malware in a sandboxed environment. It is performed after static analysis reaches a dead end due to obfuscation/ packing or other complexities. Dynamic analysis should be perfomed very carefully because it can put the system and the network at risk.
	Some famous malware sandboxes are: Any.run, Joe Sandbox, Norman sandbox, GFI sandbox, Thread Expert, Bit Blaze, etc.

## Process Monitor
A tool that can be used to monitor certain registry, file system, network, process and thread activity.

## Process Explorer
A tool that can be used to get a list of active processes, DLLs loaded by them, killing processes, log out users, etc.

## Process Replacement / Process Hollowing
Process replacement involves running a process on the system and overwriting its memory space with a malicious executable. It provides the malware with the same privileges as the process it is replacing. The strings tab present in Process Explorer can be used to detect process hollowing ( by comparing the strings contained in the disk executable image against the strings present in memory for the same executable running in memory).

## Dependency Walker
Process Explorer allows you to launch `Dependency Walker`(depends.exe) on a running process. It contains an option to find a DLL which can be used to find out all the running processes that are using a malicious DLL.

## Regshot
A tool that allows you to take and compare two registry snapshots.Take two registry snapshots, one before running the malware and one after running the malware and then compare them to find out the changes made to the registry.

Malwares often creates mutexes to prevent multiple instances of the malware from attempting to infect the same macine. If an instance of the malware is unable to obtain a handle to the named mutex, then the malware will exit.