#include <exception>
#include "App.h"
#include "Exception.h"
#include "Log.h"
#include "SPTracer.h"
#include "Window.h"

App::App(std::string fileName)
{
#ifdef _WIN64
	std::string title = "SPTracer (64-bit) - [ " + fileName + " ]";
#else
	std::string title = "SPTracer (32-bit) - [ " + fileName + " ]";
#endif

#ifdef _DEBUG
	unsigned int numThreads = 1;
#else
	unsigned int numThreads = 6;
#endif

	unsigned int width = 500;
	unsigned int height = 500;

	try
	{
		window_ = std::make_unique<Window>(width, height, title.c_str());
		tracer_ = std::make_unique<SPTracer::SPTracer>(fileName, numThreads, width, height);
		tracer_->SetImageUpdater(window_->GetImageUpdater());
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

	tracer_->Run();

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

bool App::IsInitialized() const
{
	return initialized_;
}
