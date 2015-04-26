#ifndef IMAGE_UPDATER_H
#define IMAGE_UPDATER_H

#include <string>
#include "Vec3.h"

namespace SPTracer
{

	class ImageUpdater
	{
	public:
		virtual ~ImageUpdater() { };

		virtual void UpdateImage(std::vector<Vec3> image, std::string status) = 0;

	protected:
		ImageUpdater() { };
	};

}

#endif
