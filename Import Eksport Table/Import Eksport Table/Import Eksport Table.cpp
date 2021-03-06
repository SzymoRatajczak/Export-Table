// Import Eksport Table.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Windows.h>
#include<string.h>


int PEDump::show_exports(void)
{
	int i;
	PIMAGE_EXPORT_DIRECTORY pEXP;
	uword* ordinals;
	ULONG32* functions;
	ULONG32* names;
	ULONG32 f_addr;
	char* c_name;

	ULONG32 rva = pPE->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	text_out("\nExport section");
	if ((pPE->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size <= 0) || (pPE->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress <= 0))
	{
		text_out("No export data was foun \n");
		return CMC_STATUS_FAILED;

	}


	pEXP = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(pPE, (PVOID)LI.MappedAddress, rva, 0);
	functions = (ULONG32*)ImageRvaToVa(pPE, (PVOID)LI.MappedAddrss, pEXP->AddressOfFunctions, 0);
	ordinals = (uword*)ImageRvaToVa(pPE, (PVOID)LI.MappedAddress, pEXP->AddresOfNameOrdinals, 0);
	names = (ULONG32*)ImageRvaToVa(pPE, (PVOID)LI.MappedAddress, pEXP->AddressOfNames, 0);


	for (i = 0; i <= pEXP->NumberOfNames; i++)
	{
		f_addr = (DWORD)(functions[ordinals[i]]) + imagebase;
		if (names[i])
		{
			c_name = (char*)ImageRvaToVa(pPE, (PVOID)LI.MappedAddress, names[i], 0);
			text_out("+[%d][0x%.08x]->%s\n", i, f_addr, c_name);
		}
		else
		{
			text_out("+[%d][0x%.08x]->Ordinal_%d\n", i, f_addr, i);

		}
	}

	text_out("FOund %d exported functions", i);
	return STATUS_OK;
}
 
