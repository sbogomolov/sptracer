#include "Window.h"
#include "WindowImageUpdater.h"

WindowImageUpdater::WindowImageUpdater(Window& window)
	: window_(window)
{
	char buf[1024];
	GetWindowTextA(window_.hwnd_, buf, 1024);
	title_ = std::string(buf);
}

void WindowImageUpdater::UpdateImage(std::vector<SPTracer::Vec3> image, std::string status)
{
	const auto& hwnd = window_.hwnd_;

	{
		// lock
		std::lock_guard<std::mutex> lock(window_.mutex_);

		// update title
		SetWindowTextA(hwnd, (title_ + ": " + status).c_str());

		// get DC
		HDC hdc = GetDC(hwnd);

		// create memory DC
		HDC hdcMem = CreateCompatibleDC(hdc);

		// select bitmap to memory DC
		SelectObject(hdcMem, window_.bitmap_);

		// draw to bitmap
		const auto& w = window_.width_;
		const auto& h = window_.height_;
		for (unsigned int i = 0; i < h; i++)
		{
			for (unsigned int j = 0; j < w; j++)
			{
				SPTracer::Vec3& c = image[i * w + j];
				SetPixel(hdcMem, j, i, RGB((byte)(c[0] * 255), (byte)(c[1] * 255), (byte)(c[2] * 255)));
			}
		}

		// delete memory DC
		DeleteDC(hdcMem);

		// release DC
		ReleaseDC(hwnd, hdc);
	}

	InvalidateRect(hwnd, nullptr, false);
}
