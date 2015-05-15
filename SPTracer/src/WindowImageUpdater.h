#ifndef WINDOW_IMAGE_UPDATER_H
#define WINDOW_IMAGE_UPDATER_H

#include <string>
#include <vector>
#include "SPTracer/ImageUpdater.h"
#include "SPTracer/Vec3.h"

class Window;

class WindowImageUpdater : public SPTracer::ImageUpdater
{
public:
	explicit WindowImageUpdater(Window& window);
	
	void UpdateImage(std::vector<SPTracer::Vec3> image, std::string status) override;

private:
	Window& window_;
	std::string title_;
};

#endif
