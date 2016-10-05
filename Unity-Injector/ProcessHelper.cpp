#include "ProcessHelper.h"
#include "Utility.h"

#include <TlHelp32.h>
#include <Shlwapi.h>

using namespace ProcessHelper;

#define PROXY_FILE "proxy.dll"

std::vector<ProcessHelper::ProcessInfo*> ProcessHelper::g_Processes;

bool ProcessInfo::HasModule(std::string moduleName)
{
	for (auto it = Modules.begin(); it != Modules.end(); it++)
		if (*it == moduleName)
			return true;
	return false;
}

void* ProcessHelper::ProcessInfo::GetModuleBase(std::string moduleName)
{
	void* ret = nullptr;

	HANDLE snapModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);
	MODULEENTRY32 me = { 0 };

	me.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(snapModule, &me))
	{
		do
		{
			if (Utility::ToString(me.szModule) == moduleName)
			{
				ret = (void*)me.modBaseAddr;
			}
		} while (Module32Next(snapModule, &me));
	}

	CloseHandle(snapModule);

	return ret;
}

void ProcessInfo::UpdateModules()
{
	Modules.clear();

	HANDLE snapModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);
	MODULEENTRY32 me = { 0 };

	me.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(snapModule, &me))
	{
		do
		{
			Modules.push_back(Utility::ToString(me.szModule));
		} while (Module32Next(snapModule, &me));
	}

	CloseHandle(snapModule);
}

std::string ProcessInfo::GetExePath()
{
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessId);
	DWORD tempSize = MAX_PATH;
	char temp[MAX_PATH];

	if (handle == ERROR || handle == INVALID_HANDLE_VALUE)
	{
		return "";
	}

	QueryFullProcessImageNameA(handle, NULL, temp, &tempSize);

	CloseHandle(handle);

	return std::string(temp);
}

bool ProcessInfo::SameArchitecture()
{
	short arch = GetArchitecture();

#ifdef _WIN64
	return arch == (short)0x8664;

#else
	return arch != (short)0x8664;
#endif
}

short ProcessInfo::GetArchitecture()
{
	short arch = 0;
	short peOffset = 0;
	char temp[2] = { 0 };

	std::ifstream input(GetExePath(), std::ios::binary);

	if (!input.is_open()) {
		return 0x14C; // Assume 32bit
	}


	input.seekg(0x3C);
	input.read(temp, sizeof(short));

	memcpy(&peOffset, temp, sizeof(short));

	input.seekg(peOffset + 4);
	input.read(temp, sizeof(short));

	memcpy(&arch, temp, sizeof(short));

	input.close();

	return arch;
}

void ProcessHelper::UpdateProcessInfo()
{
	// Clear list
	for (auto it = ProcessHelper::g_Processes.begin(); it != ProcessHelper::g_Processes.end(); it++) {
		delete *it;
	}

	ProcessHelper::g_Processes.clear();

	HANDLE snapProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe = { 0 };

	pe.dwSize = sizeof(PROCESSENTRY32);

	if(Process32First(snapProcess, &pe))
		do
		{
			ProcessInfo* pi = new ProcessInfo;

			pi->ProcessName = Utility::ToString(pe.szExeFile);
			pi->ProcessId = pe.th32ProcessID;

			pi->UpdateModules();

			g_Processes.push_back(pi);
		} while (Process32Next(snapProcess, &pe));

	CloseHandle(snapProcess);
}

bool ProcessHelper::IsProxyLoaded(ProcessInfo *pi)
{
	pi->UpdateModules();
	
	for (auto it = pi->Modules.begin(); it != pi->Modules.end(); it++)
		if (*it == PROXY_FILE)
			return true;
	return false;
}

void* ProcessHelper::LoadProxy(std::string proxyPath, ProcessInfo *pi)
{
	if (IsProxyLoaded(pi))
		return pi->GetModuleBase(PROXY_FILE);

	// Load via LoadLibrary + CreateRemoteThread
	// TODO: add more injection methods

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi->ProcessId);

	if (handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FARPROC fpLoadLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

	if (fpLoadLibrary == NULL)
	{
		CloseHandle(handle);
		return false;
	}

	LPVOID pDllPath = VirtualAllocEx(handle, NULL, proxyPath.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (pDllPath == NULL)
	{
		CloseHandle(handle);
		return false;
	}

	if (WriteProcessMemory(handle, pDllPath, proxyPath.data(), proxyPath.length(), NULL) == ERROR)
	{
		CloseHandle(handle);
		return false;
	}

	if (CreateRemoteThread(handle, NULL, NULL, (LPTHREAD_START_ROUTINE)fpLoadLibrary, pDllPath, NULL, NULL) == ERROR)
	{
		CloseHandle(handle);
		return false;
	}

	CloseHandle(handle);

	Sleep(1000);

	return pi->GetModuleBase(PROXY_FILE);
}

// Injects proxy dll and loads DLL via mono
bool ProcessHelper::InjectDLL(std::string dllPath, std::string dllNamespace, ProcessInfo *pi)
{
	std::string proxyPath = Utility::CurrentPath() + "\\" + PROXY_FILE;

	void* proxyBase = LoadProxy(proxyPath, pi);
	if (proxyBase == nullptr)
		return false;
	
	std::vector<char> proxyFile = Utility::FileReadAllBytes(Utility::ToWString(proxyPath));

	typedef void(__cdecl *LoadAssembly_t)(const char* assemblyPath);
	LoadAssembly_t loadAssembly = (LoadAssembly_t)((intptr)proxyBase + (intptr)Utility::GetProcAddressEx(&proxyFile[0], "LoadAssembly"));

	// Call the LoadAssembly function via CRT

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi->ProcessId);

	if (handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	LPVOID pDllStruct = VirtualAllocEx(handle, NULL, sizeof(LPVOID) * 2, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	LPVOID pDllPath = VirtualAllocEx(handle, NULL, dllPath.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	LPVOID pDllNamespace = VirtualAllocEx(handle, NULL, dllNamespace.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (pDllStruct == NULL || pDllPath == NULL || pDllNamespace == NULL)
	{
		CloseHandle(handle);
		return false;
	}

	if (WriteProcessMemory(handle, pDllStruct, &pDllPath, sizeof(LPVOID), NULL) == ERROR)
	{
		CloseHandle(handle);
		return false;
	}

	if (WriteProcessMemory(handle, (LPVOID)((intptr)pDllStruct + sizeof(LPVOID)), &pDllNamespace, sizeof(LPVOID), NULL) == ERROR)
	{
		CloseHandle(handle);
		return false;
	}

	if (WriteProcessMemory(handle, pDllPath, dllPath.data(), dllPath.length(), NULL) == ERROR)
	{
		CloseHandle(handle);
		return false;
	}

	if (WriteProcessMemory(handle, pDllNamespace, dllNamespace.data(), dllNamespace.length(), NULL) == ERROR)
	{
		CloseHandle(handle);
		return false;
	}

	if (CreateRemoteThread(handle, NULL, NULL, (LPTHREAD_START_ROUTINE)loadAssembly, pDllStruct, NULL, NULL) == ERROR)
	{
		CloseHandle(handle);
		return false;
	}

	CloseHandle(handle);

	return true;
}

