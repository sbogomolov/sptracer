#ifndef APP_H
#define APP_H

#include "stdafx.h"
#include "Config.h"

namespace SPTracer
{
	class Tracer;
}

class Window;

class App
{
public:
	explicit App(std::string configFile);
	virtual ~App();

	int Run();
	bool IsInitialized() const;

private:
	bool initialized_ = false;
	std::unique_ptr<SPTracer::Tracer> tracer_;
	std::unique_ptr<Window> window_;

	Config ReadConfig(std::string configFile) const;

};

#endif
