#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <Windows.h>
#include <Psapi.h>

namespace ProcessHelper {
	class ProcessInfo {
	public:
		std::string ProcessName;
		int ProcessId;
		std::vector<std::string> Modules;

		// Checks if process has module loaded
		bool HasModule(std::string moduleName);

		void* GetModuleBase(std::string moduleName);

		// Refreshes modules list
		void UpdateModules();

		// Returns full process file path
		std::string GetExePath();

		// true if same arch as injector
		bool SameArchitecture();

	private:
		short GetArchitecture();
	};

	extern std::vector<ProcessHelper::ProcessInfo*> g_Processes;

	void UpdateProcessInfo();

	bool IsProxyLoaded(ProcessInfo *pi);

	/// Proxy module base
	void* LoadProxy(std::string proxyPath, ProcessInfo *pi);

	bool InjectDLL(std::string dllPath, std::string dllNamespace, ProcessInfo *pi);

}