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

	try
	{
		window_ = std::make_unique<Window>(width, height, title);
		tracer_ = std::make_unique<SPTracer::SPTracer>(fileName);
	}
	catch (SPTracer::Exception e)
	{
		MessageBoxA(window_->hwnd(), e.what(), "Error", MB_ICONERROR | MB_OK);
		return;
	}

	initialized_ = true;
}

App::~App()
{
}

int App::Run()
{
	if (!initialized_)
	{
		auto s = "Trying to run not initialized application";
		SPTracer::Log::Error(s);
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
