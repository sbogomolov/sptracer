#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <windows.h>
#include "ImageUpdater.h"
#include "Vec3.h"

class Window
{
	friend class ImageUpdater;

public:
	class ImageUpdater : public SPTracer::ImageUpdater
	{
	public:
		explicit ImageUpdater(Window& window);
		void UpdateImage(std::vector<SPTracer::Vec3> image, std::string status) override;

	private:
		Window& window_;
		std::string title_;
	};

	Window(unsigned int width, unsigned int height, std::string title);
	virtual ~Window();
	
	const HWND& hwnd() const;
	std::shared_ptr<ImageUpdater> imageUpdater();

private:
	HWND hwnd_;
	HBITMAP bitmap_;

	const unsigned int width_;
	const unsigned int height_;
	std::mutex mutex_;
	std::shared_ptr<ImageUpdater> imageUpdater_;

	static std::string NewWindowClassName();
	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Paint();
};

#endif
