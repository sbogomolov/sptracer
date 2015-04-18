#include <cstdlib>
#include <exception>
#include <sstream>
#include "Window.h"
#include "Log.h"

namespace sptracer
{
	Window::Window(int width, int height, std::string title, HINSTANCE hInstance)
	{
		// generate new window class name
		auto className = NewWindowClassName();

		// register window class
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC) WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className.c_str();
		wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			Log::Info("Could not register window class");
			throw std::exception("Could not register window class");
		}

		// create window
		_hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			className.c_str(),
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			nullptr, nullptr, hInstance, nullptr);

		if (_hwnd == nullptr)
		{
			Log::Error("Could not create window");
			throw std::exception("Could not create window");
		}

		// show window
		ShowWindow(_hwnd, SW_SHOWNORMAL);
		UpdateWindow(_hwnd);

		Log::Trace("Window created");
	}

	Window::~Window()
	{
		Log::Trace("Window destroyed");
	}

	std::string Window::NewWindowClassName()
	{
		static const int len = 16;
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		std::ostringstream ss;
		for (int i = 0; i < len; ++i) {
			ss << alphanum[std::rand() % (sizeof(alphanum) - 1)];
		}

		return ss.str();
	}

	LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		return 0;
	}

}