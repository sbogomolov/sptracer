#ifndef APP_H
#define APP_H

#include <memory>
#include <string>
#include <windows.h>

namespace SPTracer
{
	class SPTracer;
}

class Window;

class App
{
public:
	App(std::string fileName);
	virtual ~App();
	int Run();

	bool IsInitialized() const;

private:
	bool initialized_ = false;
	std::unique_ptr<SPTracer::SPTracer> tracer_;
	std::unique_ptr<Window> window_;
};

#endif
