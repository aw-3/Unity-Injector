// proxy.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

std::queue<std::pair<std::string, std::string>> g_AssemblyQueue;

void Entrypoint(HMODULE hModule)
{
	LoadHook();

	// RemoveHook();
}

void LoadAssembly(assembly_params* assembly)
{
	std::string dllPath = assembly->szPath;
	std::string dllNamespace = assembly->szNamespace;

	std::pair<std::string, std::string> dllInfo(dllPath, dllNamespace);

	g_AssemblyQueue.push(dllInfo);

}
