#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <memory>
#include <mutex>
#include <string>

class WindowImageUpdater;

class Window
{
	friend class WindowImageUpdater;

public:
	Window(unsigned int width, unsigned int height, std::string title);
	virtual ~Window();
	
	const HWND& hwnd() const;
	std::shared_ptr<WindowImageUpdater> imageUpdater();

private:
	HWND hwnd_;
	HBITMAP bitmap_;

	const unsigned int width_;
	const unsigned int height_;
	std::mutex mutex_;
	std::shared_ptr<WindowImageUpdater> imageUpdater_;

	static std::string NewWindowClassName();
	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Paint();
};

#endif
