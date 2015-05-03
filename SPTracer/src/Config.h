#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <string>
#include "Camera.h"

struct Config
{
	enum class ModelType
	{
		MDLA,
		OBJ
	};

	ModelType modelType;
	std::string modelFile;
	std::unique_ptr<SPTracer::Camera> camera;
	bool cameraLoaded;
	unsigned int width;
	unsigned int height;
	unsigned int numThreads;
	unsigned int waveLengthMin;
	unsigned int waveLengthMax;
	unsigned int waveLengthStep;
};

#endif
