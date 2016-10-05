#pragma once

// returned.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <cstdint>

#define MonoDomain void
#define MonoAssembly void
#define MonoClass void
#define MonoMethod void
#define MonoObject void
#define MonoImage void
#define MonoProperty void

typedef enum {
	MONO_IMAGE_OK,
	MONO_IMAGE_ERROR_ERRNO,
	MONO_IMAGE_MISSING_ASSEMBLYREF,
	MONO_IMAGE_IMAGE_INVALID
} MonoImageOpenStatus;



std::vector<char> FileReadAllBytes(const std::wstring& name);

template<class T>
T MONO_FUNC(const char *funcName);

//Class
typedef MonoClass* (__cdecl* mClassFromName_t)(MonoImage* image, const char* name_space, const char* name);
typedef MonoMethod* (__cdecl* mClassGetMethodFromName_t)(MonoClass* mclass, const char* name, int param_count);
typedef MonoObject* (__cdecl* mRuntimeInvoke_t)(MonoMethod* method, void* obj, void** params, MonoObject** exc);
typedef MonoProperty* (__cdecl* mClassGetPropertyFromName_t)(MonoClass* mclass, const char* name);
typedef MonoMethod* (__cdecl* mPropertyGetSetMethod_t)(MonoProperty* prop);
typedef MonoMethod* (__cdecl* mPropertyGetGetMethod_t)(MonoProperty* prop);
typedef MonoAssembly* (__cdecl* mAssemblyOpen_t)(const char *filename, MonoImageOpenStatus *status);
typedef MonoImage* (__cdecl* mAssemblyGetImage_t)(MonoAssembly *assembly);
typedef MonoDomain* (__cdecl* mDomainGet_t)();
typedef MonoImage* (__cdecl* mImageOpenFromData_t)(char* data, uint32_t len, int32_t nCopy, MonoImageOpenStatus *status, int32_t refonly);
typedef MonoAssembly* (__cdecl* mAssemblyLoadFromFull_t)(MonoImage* image, void *fname, MonoImageOpenStatus *status, bool refonly);


void LoadHook();
void RemoveHook();

void LoadMonoDll(std::string dllPath, std::string dllNamespace);