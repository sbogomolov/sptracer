#include "App.h"
#include "Log.h"

namespace sptracer
{
	HINSTANCE App::_hInstance = nullptr;

	App::App()
	{
		Log::Trace("Application created");
		_window = std::make_unique<Window>(500, 500, _title, _hInstance);
	}

	void App::Init(HINSTANCE hInstance)
	{
		_hInstance = hInstance;
	}

	App& App::GetInstance()
	{
		static App app;
		return app;
	}

	int App::Run()
	{
		Log::Info("SPTracer started");

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Log::Info("SPTracer closed");

		return (int)msg.wParam;
	}

	App::~App()
	{
		Log::Trace("Application destroyed");
	}
}