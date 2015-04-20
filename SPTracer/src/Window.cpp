#include <cstdlib>
#include <exception>
#include <sstream>
#include "Log.h"
#include "Window.h"

Window::Window(int width, int height, std::string title)
{
	// get HINSTANCE
	auto hinstance = GetModuleHandle(nullptr);

	// generate new window class name
	auto className = NewWindowClassName();

	// register window class
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)StaticWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(Window*);
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className.c_str();
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		SPTracer::Log::Error("Could not register window class");
		throw std::exception("Could not register window class");
	}

	// create window
	hwnd_ = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className.c_str(),
		title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, hinstance,
		static_cast<LPVOID>(this));

	if (hwnd_ == nullptr)
	{
		SPTracer::Log::Error("Could not create window");
		throw std::exception("Could not create window");
	}

	// show window
	ShowWindow(hwnd_, SW_SHOWNORMAL);
	UpdateWindow(hwnd_);
}

Window::~Window()
{
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

LRESULT CALLBACK Window::StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// store instance pointer while handling the first message
	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		LPVOID ptr = cs->lpCreateParams;
		SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(ptr));
	}

	// at this point the instance pointer will always be available
	Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, 0));
		
	// return instance window proc
	return wnd->WindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
