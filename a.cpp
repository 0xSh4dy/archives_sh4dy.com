#include <Windows.h>
#include <winternl.h>
#include <stdio.h>

int main(int argc, char const* argv[]) {
    PPEB pPeb = 0;
    PLDR_DATA_TABLE_ENTRY pLdrDataTableEntry = 0;

#ifdef _WIN64
    pPeb = (PPEB)__readgsdword(0x60);
#else
    pPeb = (PPEB)__readfsdword(0x30);
#endif

    PLIST_ENTRY pLE = &(pPeb->Ldr->InMemoryOrderModuleList);
    for (PLIST_ENTRY pl = pLE->Flink; pl != pLE; pl = pl->Flink)
    {
        pl = pl - 1;
        pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)pl;
        printf("\n %ls\t%x %d", pLdrDataTableEntry->FullDllName.Buffer, (DWORD)pLdrDataTableEntry->DllBase, (int)pl);
        pl = pl + 1;
    }
    return 0;
}