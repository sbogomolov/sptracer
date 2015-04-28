#ifndef CAMERA_H
#define CAMERA_H

#include "../Vec3.h"

namespace SPTracer
{

	struct Camera
	{
		std::string name;	// camera name
		Vec3 p;				// center of projection
		Vec3 n;				// image plane normal
		Vec3 up;			// up direction
		float f;			// distance to image plane
		float iw;			// image width
		float ih;			// image height
		float icx;			// image center x
		float icy;			// image center y
		float t;			// time of exposure
	};

}

#endif
