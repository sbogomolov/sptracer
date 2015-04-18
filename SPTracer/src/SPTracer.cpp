#include <windows.h>
#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// init the application
	sptracer::App::Init(hInstance);

	// run the application
	return sptracer::App::GetInstance().Run();
}