#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <windows.h>

class Window
{
public:
	Window(int width, int height, std::string title);
	virtual ~Window();
	
	inline const HWND hwnd() const { return hwnd_; };

private:
	HWND hwnd_;

	static std::string NewWindowClassName();
	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif
