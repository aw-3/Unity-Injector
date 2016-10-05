#include "Utility.h"
#include <Windows.h>
#include <locale>
#include <vector>
#include <fstream>
#include <codecvt>
#include <Shlwapi.h>
#include <msclr\marshal_cppstd.h>

using namespace Utility;

std::string Utility::ToString(std::wstring wstr) {
	using ccvt = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<ccvt, wchar_t> converter;

	return converter.to_bytes(wstr);
}

std::string Utility::ToString(System::String^ str) {
	msclr::interop::marshal_context context;
	return context.marshal_as<std::string>(str);
}

std::wstring Utility::ToWString(std::string str) {
	using ccvt = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<ccvt, wchar_t> converter;

	return converter.from_bytes(str);
}

std::string Utility::CurrentPath()
{
	char pBuf[MAX_PATH] = { 0 };

	int bytes = GetModuleFileNameA(NULL, pBuf, MAX_PATH);
	
	if (bytes > 0)
	{
		std::string ret = std::string(pBuf);

		ret = ret.substr(0, ret.size() - 19); // UGLY - HACK

		return ret;
	}
		
	return "";
}

template <class T>
LPVOID GetPtrFromRVA(
	DWORD rva, T* pNTHeader, PBYTE imageBase) // 'T' = PIMAGE_NT_HEADERS
{
	PIMAGE_SECTION_HEADER pSectionHdr;
	INT delta;

	pSectionHdr = GetEnclosingSectionHeader(rva, pNTHeader);
	if (!pSectionHdr)
		return 0;

	delta = (INT)(pSectionHdr->VirtualAddress - pSectionHdr->PointerToRawData);
	return (PVOID)(imageBase + rva - delta);
}

template <class T>
PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(
	DWORD rva,
	T* pNTHeader)   // 'T' == PIMAGE_NT_HEADERS
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);
	unsigned i;

	for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++)
	{
		// This 3 line idiocy is because Watcom's linker actually sets the
		// Misc.VirtualSize field to 0.  (!!! - Retards....!!!)
		DWORD size = section->Misc.VirtualSize;
		if (0 == size)
			size = section->SizeOfRawData;

		// Is the RVA within this section?
		if ((rva >= section->VirtualAddress) &&
			(rva < (section->VirtualAddress + size)))
			return section;
	}

	return 0;
}

void* Utility::GetProcAddressEx(void* pModuleBase, std::string procName)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pModuleBase;

	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return nullptr;

	PIMAGE_NT_HEADERS32 pNtHdrs32 = (PIMAGE_NT_HEADERS32)((PUCHAR)pModuleBase + pDosHeader->e_lfanew);
	PIMAGE_NT_HEADERS64 pNtHdrs64 = (PIMAGE_NT_HEADERS64)((PUCHAR)pModuleBase + pDosHeader->e_lfanew);

	PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;

	PULONG	pAddressOfFunctions;
	PULONG	pAddressOfNames;
	PUSHORT	pAddressOfOrdinals;
	ULONG	numberOfNames;

	if (pNtHdrs32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		pExportDir = (PIMAGE_EXPORT_DIRECTORY)GetPtrFromRVA<IMAGE_NT_HEADERS32>(pNtHdrs32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, pNtHdrs32, (PBYTE)pModuleBase);
		
		pAddressOfFunctions = (PULONG)GetPtrFromRVA<IMAGE_NT_HEADERS32>(pExportDir->AddressOfFunctions, pNtHdrs32, (PBYTE)pModuleBase);
		pAddressOfNames = (PULONG)GetPtrFromRVA<IMAGE_NT_HEADERS32>(pExportDir->AddressOfNames, pNtHdrs32, (PBYTE)pModuleBase);
		pAddressOfOrdinals = (PUSHORT)GetPtrFromRVA<IMAGE_NT_HEADERS32>(pExportDir->AddressOfNameOrdinals, pNtHdrs32, (PBYTE)pModuleBase);
		numberOfNames = pExportDir->NumberOfNames;

		for (ULONG i = 0; i < numberOfNames; i++) {
			
			PCSTR pszName = (PCSTR)((PUCHAR)pModuleBase + (int)GetPtrFromRVA<IMAGE_NT_HEADERS32>(pAddressOfNames[i], pNtHdrs32, (PBYTE)pModuleBase));
			SHORT ordinal = pAddressOfOrdinals[i];
			if (procName == pszName) {
				return GetPtrFromRVA<IMAGE_NT_HEADERS32>(pAddressOfFunctions[ordinal], pNtHdrs32, (PBYTE)pModuleBase);
			}
		}
	}
	else if (pNtHdrs64->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		pExportDir = (PIMAGE_EXPORT_DIRECTORY)GetPtrFromRVA<IMAGE_NT_HEADERS64>(pNtHdrs64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, pNtHdrs64, (PBYTE)pModuleBase);
	
		pAddressOfFunctions = (PULONG)GetPtrFromRVA<IMAGE_NT_HEADERS64>(pExportDir->AddressOfFunctions, pNtHdrs64, (PBYTE)pModuleBase);
		pAddressOfNames = (PULONG)GetPtrFromRVA<IMAGE_NT_HEADERS64>(pExportDir->AddressOfNames, pNtHdrs64, (PBYTE)pModuleBase);
		pAddressOfOrdinals = (PUSHORT)GetPtrFromRVA<IMAGE_NT_HEADERS64>(pExportDir->AddressOfNameOrdinals, pNtHdrs64, (PBYTE)pModuleBase);
		numberOfNames = pExportDir->NumberOfNames;

		for (ULONG i = 0; i < numberOfNames; i++) {

			PCSTR pszName = (PCSTR)(GetPtrFromRVA<IMAGE_NT_HEADERS64>(pAddressOfNames[i], pNtHdrs64, (PBYTE)pModuleBase));
			SHORT ordinal = pAddressOfOrdinals[i];
			if (/*procName == pszName*/ std::string(pszName).find(procName) != std::string::npos) { // HACK
				return (void*)pAddressOfFunctions[ordinal];
			}
		}
	}
		

	

	
	return nullptr;
}

std::vector<char> Utility::FileReadAllBytes(const std::wstring& name) {
	std::ifstream input(name, std::ios::binary);
	if (input.is_open()) {

		std::vector<char> buffer((
			std::istreambuf_iterator<char>(input)),
			(std::istreambuf_iterator<char>()));

		return buffer;
	}
	return std::vector<char>();
}