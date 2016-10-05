#pragma once

#include <string>
#include <vector>

#ifdef _WIN64
typedef uint64_t intptr;
#else
typedef uint32_t intptr;
#endif

namespace Utility {
	std::string ToString(std::wstring);
	std::string ToString(System::String^);

	std::wstring ToWString(std::string);

	std::string CurrentPath();

	void* GetProcAddressEx(void* pModuleBase, std::string procName);

	std::vector<char> FileReadAllBytes(const std::wstring& name);
}