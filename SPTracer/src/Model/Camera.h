#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"

namespace sptracer
{

	struct Camera
	{
		std::string name;	// camera name
		Vec3 p;				// center of projection
		Vec3 n;				// image plane normal
		Vec3 up;			// up direction
		double f;			// distance to image plane
		double iw;			// image width
		double ih;			// image height
		double icx;			// image center x
		double icy;			// image center y
		double t;			// time of exposure
	};

}

#endif
