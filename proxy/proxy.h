#pragma once

extern std::queue<std::pair<std::string, std::string>> g_AssemblyQueue;

struct assembly_params {
	const char* szPath;
	const char* szNamespace;
};

void Entrypoint(HMODULE hModule);

__declspec(dllexport) void LoadAssembly(assembly_params* assembly);