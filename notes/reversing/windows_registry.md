# Windows Registry
The Windows registry is a hierarchical database that stores low-level settings, information, options,etc. for the OS and applications that use it. Malwares use the registry for persistence or configuration data.

- Registry Editor (Regedit) is a tool that can be used to view and edit the Windows registry. 
- A hive is a logical group of keys, sub-keys and their values.
## Root Key
The registry contains five main root keys:
```
1. HKEY_CLASSES_ROOT (HKCR)
2. HKEY_CURRENT_USER (HKCU)
3. HKEY_LOCAL_MACHINE (HKLM)
4. HKEY_USERS (HKU)
5. HKEY_CURRENT_CONFIG (HKCC)
```
`HKEY` stands for `Handle to Registry Key`.

1. HKEY_CLASSES_ROOT
The keys under HKCR store a variety of subkey types, such as filename extension associations and COM registration information. HKCR is a reference to `HKEY_LOCAL_MACHINE\Software\Classes`.

2. HKEY_CURRENT_USER
It stores settings specific to the current user.

3. HKEY_LOCAL_MACHINE
It stores settings that are global to the local machine such as the configuration data used by all the users in windows.

4. HKEY_USERS
It contains information about all the users that logon to the computer (default users, new user and the current users).

5. HKEY_CURRENT_CONFIG
It stores settings about the current hardware configuration, specially differences between the current and the standard configuration. HKCC is an alias for `HKEY_LOCAL_MACHINE\System\CurrentControlSet\Hardware Profiles\Current`. Information about the standard hardware configuration is stored under the `Software` and `System` keys of `HKEY_LOCAL_MACHINE`.

## Some interesting registry keys

### Run and RunOnce
`HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run`
`HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunOnce`

The `Run` key makes a program run every time the user logs on, while the `RunOnce` keys makes the program run only once, when the user logs on and then the key is deleted.

## AppInit DLLs
`User32.dll` is a dll responsible for providing the core functionalities in user interfaces such as management, input passing, input processing and standard controls. This DLL loads several other DLLs that are specified in 

Adding a dll in `HKEY_LOCAL_MACHINE\Software\Microsoft\WindowsNT\CurrentVersion\Windows\AppInit_DLLs` for 32 bit systems and `HKEY_LOCAL_MACHINE\Software\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Windows` for 64 bit would load it whenever an application using `User32.dll` is run. To perform this operation, the value of 
`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\LoadAppInit_DLLs` must be 1.

## DLL Search Order Hijacking
In many cases, applications specify the path to the DLL it needs to load but if the path isn't specified, there's a specific order that the application will follow to find out the DLL. The DLL search order is:
The directory from which the application is loaded -> The system directory( System32, can be found using`GetSystemDirectory()`) -> The 16 bit system directory(System) -> The windows directory (Windows, can be found using `GetWindowsDirectory()`) -> The current directory -> directories listed in PATH env variable.
This order is followed if the `SafeDLLSearchMode` is enabled. If it is disabled, the current directory escalates to the second place. This can be set using `HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\SafeDllSearchMode`. 

DLL search order hijacking occurs by placing a malicious DLL into one of the folders mentioned above to make sure that the malicious DLL is found before the legitimate one. If the DLL is on the list of known DLLs, `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\KnownDLLs`, the system uses its copy of the known DLL instead of searching for it.

## User Logon
`HKLM\Software\Microsoft\WindowsNT\CurrentVersion\Winlogon\UserInit` is used to launch `userinit.exe`. This key can be modified to point to some different executable.

## Screensavers (PE files with .scr extension)
They are programs that execute after a configurable time of user inactivity. `HKEY_CURRENT_USER\Control Panel\Desktop\ScreenSaveActive` should be set 1 to enable the screensaver. The path to the screensaver program(.scr) is controlled by `HKEY_CURRENT_USER\Control Panel\Desktop\SCRNSAVE.EXE` . This value can be modified to execute some unwanted program.

## File extensions
By default, windows opens `.txt` files using notepad. Again, the application used to open a file is specified in the windows registry. For example, replacing the path of `notepad.exe` with some other executable in `HKEY_CLASSES_ROOT\txtfile\shell\open\command` would run that executable instead of notepad.exe whenever a `.txt` file is double clicked by the user.

## Uninstall
The UninstallString field `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\<application_name>` stores the path to the uninstaller for the application specified by `application_name` which is executed whenever the user tries to uninstall an application.

## Event Viewer Help Link
The `MicrosoftRedirectionURL` field present in `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Event Viewer` stores a url that is opened whenever user decides to view the help menu. Inserting the path to a local file (file://path) into `MicrosoftRedirectionURL` will execute that file instead of opening the url.

- Some common functions used to manipulate the registry are `RegOpenKeyEx`,`RegSetValueEx`,`RegGetValue`
References: 
https://cocomelonc.github.io/
https://en.wikipedia.org/wiki/Windows_Registry
https://tweaklibrary.com/what-are-registry-root-keys/
https://pentestlab.blog/2020/01/07/persistence-appinit-dlls/
https://sensei-infosec.netlify.app/forensics/registry/persistence/2020/04/15/malware-persistence-registry.html
https://redcanary.com/threat-detection-report/techniques/dll-search-order-hijacking/
https://book.hacktricks.xyz/windows-hardening/windows-local-privilege-escalation/dll-hijacking
