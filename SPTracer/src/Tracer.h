#ifndef SPT_TRACER_H
#define SPT_TRACER_H

#include "stdafx.h"
#include "Camera.h"
#include "PixelData.h"
#include "Spectrum.h"

namespace SPTracer
{
	struct Ray;
	class Vec3;
	class XYZConverter;
	class RGBConverter;
	class ImageUpdater;
	class Model;
	class TaskScheduler;
	
	class Tracer
	{
		friend class TraceTask;

	public:
		Tracer(std::unique_ptr<Model> model, Camera camera,
			unsigned int width, unsigned int height, unsigned int numThreads,
			Spectrum spectrum);
		virtual ~Tracer();

		void Run();
		void AddSamples(std::vector<Vec3>& color);
		void SetImageUpdater(std::shared_ptr<ImageUpdater> imageUpdater);
		void UpdateImage();

	private:
		std::mutex mutex_;
		unsigned int numThreads_;
		unsigned int width_;
		unsigned int height_;
		unsigned long pixelsCount_;
		unsigned long completedPasses_ = 0;
		Spectrum spectrum_;
		Camera camera_;
		std::unique_ptr<Model> model_;
		std::unique_ptr<TaskScheduler> taskScheduler_;
		std::unique_ptr<XYZConverter> xyzConverter_;
		std::unique_ptr<RGBConverter> rgbConverter_;
		std::shared_ptr<ImageUpdater> imageUpdater_;
		std::chrono::high_resolution_clock::time_point start_;
		std::vector<PixelData> pixels_;

		float FindExposure(const std::vector<Vec3>& xyzColor) const;
		float Clamp(float c) const;
		std::vector<Vec3> Tonemap(const std::vector<Vec3>& xyzColor) const;
		std::string FormatNumber(float n) const;
	};

}

#endif
