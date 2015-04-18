#ifndef SPT_WINDOW_H
#define SPT_WINDOW_H

#include <string>
#include <windows.h>

namespace sptracer
{

	class Window
	{
	public:
		Window(int width, int height, std::string title, HINSTANCE hInstance);
		virtual ~Window();

	private:
		HWND _hwnd;

		static std::string NewWindowClassName();
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};

}

#endif
