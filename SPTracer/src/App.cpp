#include <exception>
#include "App.h"
#include "Exception.h"
#include "Log.h"
#include "SPTracer.h"
#include "Window.h"

App::App(std::string fileName, int width, int height)
{
	std::string title =
#ifdef _WIN64
		"SPTracer (64-bit) - [ " + fileName + " ]";
#else
		"SPTracer (32-bit) - [ " + fileName + " ]";
#endif

	try
	{
		window_ = std::make_unique<Window>(width, height, title.c_str());
		tracer_ = std::make_unique<SPTracer::SPTracer>(fileName, 4, width, height);
	}
	catch (SPTracer::Exception e)
	{
		MessageBoxA(window_->GetHwnd(), e.what(), "Error", MB_ICONERROR | MB_OK);
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

bool App::IsInitialized() const
{
	return initialized_;
}
