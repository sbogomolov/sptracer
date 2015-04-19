#include <exception>
#include "App.h"
#include "Exception.h"
#include "Log.h"

App::App(std::string fileName, int width, int height)
{
	auto title =
#ifdef _WIN64
	"SPTracer (64-bit)";
#else
	"SPTracer (32-bit)";
#endif

	sptracer::Log::Trace("Application created");

	try
	{
		window_ = std::make_unique<Window>(width, height, title);
		tracer_ = std::make_unique<sptracer::SPTracer>(fileName);
	}
	catch (sptracer::Exception e)
	{
		MessageBoxA(window_->hwnd(), e.what(), "Error", MB_ICONERROR | MB_OK);
		return;
	}

	initialized_ = true;
}

int App::Run()
{
	if (!initialized_)
	{
		auto s = "Trying to run not initialized application";
		sptracer::Log::Error(s);
		throw std::exception(s);
	}

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

App::~App()
{
	sptracer::Log::Trace("Application destroyed");
}
