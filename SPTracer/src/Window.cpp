#include <exception>
#include "Window.h"
#include "WindowImageUpdater.h"
#include "SPTracer/Log.h"

Window::Window(unsigned int width, unsigned int height, std::string title)
	: width_(width), height_(height)
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
		throw std::runtime_error("Could not register window class");
	}

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	// compute window size
	RECT rect;
	SetRect(&rect, 0, 0, width_, height_);
	AdjustWindowRectEx(&rect, style, false, exStyle);

	// create window
	hwnd_ = CreateWindowEx(
		exStyle,
		className.c_str(),
		title.c_str(),
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, hinstance,
		static_cast<LPVOID>(this));

	if (hwnd_ == nullptr)
	{
		SPTracer::Log::Error("Could not create window");
		throw std::runtime_error("Could not create window");
	}

	// show window
	ShowWindow(hwnd_, SW_SHOWNORMAL);
	UpdateWindow(hwnd_);

	// create bitmap in memory
	HDC hdc = GetDC(hwnd_);
	bitmap_ = CreateCompatibleBitmap(hdc, width_, height_);
	ReleaseDC(hwnd_, hdc);

	// create image upadter
	imageUpdater_ = std::make_shared<WindowImageUpdater>(*this);
}

Window::~Window()
{
	DeleteObject(bitmap_);
}

const HWND& Window::hwnd() const
{
	return hwnd_;
}

std::shared_ptr<WindowImageUpdater> Window::imageUpdater()
{
	return imageUpdater_;
}

std::string Window::NewWindowClassName()
{
	static const unsigned int len = 16;
	static const std::string alphanum =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::ostringstream ss;
	for (size_t i = 0; i < len; i++)
	{
		ss << alphanum[std::rand() % (alphanum.length() - 1)];
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
	case WM_PAINT:
		Paint();
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void Window::Paint()
{
	// lock
	std::lock_guard<std::mutex> lock(mutex_);

	// get DC
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd_, &ps);

	// create memory DC
	HDC hdcMem = CreateCompatibleDC(hdc);

	// select bitmap to memory DC
	SelectObject(hdcMem, bitmap_);

	// copy bitmap from memory to window
	BitBlt(hdc, 0, 0, width_, height_, hdcMem, 0, 0, SRCCOPY);

	// delete memory DC
	DeleteDC(hdcMem);

	// release DC
	EndPaint(hwnd_, &ps);
}
