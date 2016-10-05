#include "Main.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace UnityInjector;

[System::STAThreadAttribute]
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Main mainFrm;
	
	Application::Run(%mainFrm);

	return 0;
}
