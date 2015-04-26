#include <cstdlib>
#include <exception>
#include <sstream>
#include "Log.h"
#include "Window.h"

Window::Window(unsigned int width, unsigned int height, std::string title)
	: width_(width), height_(height)
{
	image_.resize(width_ * height_);

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
		throw std::exception("Could not create window");
	}

	// show window
	ShowWindow(hwnd_, SW_SHOWNORMAL);
	UpdateWindow(hwnd_);

	// create image upadter
	imageUpdater_ = std::make_shared<Window::ImageUpdater>(image_, mutex_, hwnd_);
}

Window::~Window()
{
}

const HWND Window::GetHwnd() const
{
	return hwnd_;
}

std::shared_ptr<Window::ImageUpdater> Window::GetImageUpdater()
{
	return imageUpdater_;
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
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd_, &ps);
	HDC hdcMem = CreateCompatibleDC(hdc);

	HBITMAP bitmap = CreateCompatibleBitmap(hdc, width_, height_);
	SelectObject(hdcMem, bitmap);
	
	{
		// lock
		std::lock_guard<std::mutex> lock(mutex_);

		for (unsigned int i = 0; i < height_; i++)
		{
			for (unsigned int j = 0; j < width_; j++)
			{
				SPTracer::Vec3& c = image_[i * width_ + j];
				SetPixel(hdcMem, j, i, RGB((byte)(c.x * 255), (byte)(c.y * 255), (byte)(c.z * 255)));
			}
		}

		BitBlt(hdc, 0, 0, width_, height_, hdcMem, 0, 0, SRCCOPY);
	}


	DeleteDC(hdcMem);
	EndPaint(hwnd_, &ps);
}

Window::ImageUpdater::ImageUpdater(std::vector<SPTracer::Vec3>& image, std::mutex& mutex, HWND& hwnd)
	: hwnd_(hwnd), image_(image), mutex_(mutex)
{
	char buf[1024];
	GetWindowTextA(hwnd_, buf, 1024);
	title_ = std::string(buf);
}

void Window::ImageUpdater::UpdateImage(std::vector<SPTracer::Vec3> image, std::string status)
{
	{
		// lock
		std::lock_guard<std::mutex> lock(mutex_);
		
		// update image
		image_ = std::move(image);

		// update title
		SetWindowTextA(hwnd_, (title_ + ": " + status).c_str());
	}

	InvalidateRect(hwnd_, nullptr, false);
}
