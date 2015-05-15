#ifndef CONFIG_H
#define CONFIG_H

#include "SPTracer/Color/Spectrum.h"
#include "SPTracer/Model/Camera.h"

struct Config
{
	enum class ModelType
	{
		MDLA,
		OBJ
	};

	ModelType modelType;
	std::string modelFile;
	SPTracer::Camera camera;
	bool cameraLoaded;
	unsigned int width;
	unsigned int height;
	unsigned int numThreads;
	SPTracer::Spectrum spectrum;
};

#endif
