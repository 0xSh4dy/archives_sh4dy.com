# IDAPython Notes

## Setting Breakpoints
```
idc.add_bpt(address,0,BPT_SOFT) # Set a software breakpoint at some address
```

## Monitor for a breakpoint's trigger
For that, we need to implement a debugger hooking class (any class that inherits `DBG_Hooks`)
```
```

## Continue process after a breakpoint hits
```
idaapi.continue_process()
```

### Patch a byte at some memory address during debugging
```
ida_bytes.patch_dbg_byte(address,value)
```