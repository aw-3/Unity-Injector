// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cstdint>

#if defined _M_X64
#if defined _DEBUG
#pragma comment(lib, "libMinHook-x64-v140-mtd.lib")
#else
#pragma comment(lib, "libMinHook-x64-v140-mt.lib")
#endif
#elif defined _M_IX86
#if defined _DEBUG
#pragma comment(lib, "libMinHook-x86-v140-mtd.lib")
#else
#pragma comment(lib, "libMinHook-x86-v140-mt.lib")
#endif
#endif

#include "MinHook.h"

#include "mono.h"
#include "proxy.h"