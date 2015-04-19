#ifndef APP_H
#define APP_H

#include <memory>
#include <windows.h>
#include "SPTracer.h"
#include "Window.h"

class App
{
public:
	App(std::string fileName, int width, int height);
	virtual ~App();
	int Run();

	inline bool initialized() const { return initialized_; };

private:
	bool initialized_ = false;
	std::unique_ptr<sptracer::SPTracer> tracer_;
	std::unique_ptr<Window> window_;
};

#endif
