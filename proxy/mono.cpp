#include "stdafx.h"

mClassFromName_t mono_class_from_name = MONO_FUNC<mClassFromName_t>("mono_class_from_name");
mClassGetMethodFromName_t mono_class_get_method_from_name = MONO_FUNC<mClassGetMethodFromName_t>("mono_class_get_method_from_name");
mRuntimeInvoke_t mono_runtime_invoke = MONO_FUNC<mRuntimeInvoke_t>("mono_runtime_invoke");
mClassGetPropertyFromName_t mono_class_get_property_from_name = MONO_FUNC<mClassGetPropertyFromName_t>("mono_class_get_property_from_name");
mPropertyGetSetMethod_t mono_property_get_set_method = MONO_FUNC<mPropertyGetSetMethod_t>("mono_property_get_set_method");
mPropertyGetGetMethod_t mono_property_get_get_method = MONO_FUNC<mPropertyGetGetMethod_t>("mono_property_get_get_method");
mAssemblyOpen_t mono_assembly_open = MONO_FUNC<mAssemblyOpen_t>("mono_assembly_open");
mAssemblyGetImage_t mono_assembly_get_image = MONO_FUNC<mAssemblyGetImage_t>("mono_assembly_get_image");
mDomainGet_t mono_domain_get = MONO_FUNC<mDomainGet_t>("mono_domain_get");
mImageOpenFromData_t mono_image_open_from_data = MONO_FUNC<mImageOpenFromData_t>("mono_image_open_from_data");
mAssemblyLoadFromFull_t mono_assembly_load_from_full = MONO_FUNC<mAssemblyLoadFromFull_t>("mono_assembly_load_from_full");

mDomainGet_t origDomainGet = nullptr;
bool injected = false;

std::vector<char> FileReadAllBytes(const std::wstring& name) {
	std::ifstream input(name, std::ios::binary);
	if (input.is_open()) {

		std::vector<char> buffer((
			std::istreambuf_iterator<char>(input)),
			(std::istreambuf_iterator<char>()));

		return buffer;
	}
	return std::vector<char>();
}

template<class T>
T MONO_FUNC(const char *funcName) {
	static HMODULE hMono = GetModuleHandleA("mono.dll");
	return (T)GetProcAddress(hMono, funcName);
}

void* __cdecl hkMonoDomainGet()
{
	void* ret = origDomainGet();

	if (!g_AssemblyQueue.empty())
	{
		auto assembly = g_AssemblyQueue.front();
		g_AssemblyQueue.pop();

		LoadMonoDll(assembly.first, assembly.second);
	}

	return ret;
}

void LoadHook()
{
	MH_Initialize();
	MH_CreateHook(mono_domain_get, &hkMonoDomainGet, (LPVOID*)&origDomainGet);
	MH_EnableHook(mono_domain_get);
}

void RemoveHook()
{
	MH_DisableHook(mono_domain_get);
	MH_Uninitialize();
}


void LoadMonoDll(std::string dllPath, std::string dllNamespace)
{
	MonoImageOpenStatus status;

	using ccvt = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<ccvt, wchar_t> converter;
	std::vector<char> data = FileReadAllBytes(converter.from_bytes(dllPath));

	MonoImage* raw_img = mono_image_open_from_data(reinterpret_cast<char*>(data.data()), data.size(), 1, &status, 0);

	MonoAssembly* assembly = mono_assembly_load_from_full(raw_img, "ok", &status, 0);

	MonoImage* img = mono_assembly_get_image(assembly);

	MonoClass* mainClass = mono_class_from_name(img, dllNamespace.data(), "Loader");

	MonoMethod* method = mono_class_get_method_from_name(mainClass, "Load", 0);

	mono_runtime_invoke(method, NULL, NULL, NULL);
}
