#ifndef SPT_SPTRACER_H
#define SPT_SPTRACER_H

#include <chrono>
#include <memory>
#include <string>
#include <mutex>
#include <vector>

namespace SPTracer
{
	struct Ray;
	struct Vec3;
	class XYZConverter;
	class RGBConverter;
	class ImageUpdater;
	class Model;
	class TaskScheduler;
	
	class SPTracer
	{
	public:
		SPTracer(std::string fileName, size_t, unsigned int width, unsigned int height);
		virtual ~SPTracer();

		const Model& GetModel() const;
		const XYZConverter& GetXYZConverter() const;
		TaskScheduler& GetTaskScheduler() const;
		double GetWaveLengthMin() const;
		double GetWaveLengthMax() const;
		double GetWaveLengthStep() const;
		size_t GetWaveLengthCount() const;
		size_t GetCompletedSamplesCount() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		void Run();
		void AddSamples(std::vector<Vec3>& radiances);
		void SetImageUpdater(std::shared_ptr<ImageUpdater> imageUpdater);
		void UpdateImage();

	private:
		std::mutex mutex_;
		size_t numThreads_;
		unsigned int width_;
		unsigned int height_;
		double waveLengthMin_;
		double waveLengthMax_;
		double waveLengthStep_;
		size_t waveLengthCount_;
		size_t completedSamplesCount_ = 0;
		std::unique_ptr<XYZConverter> xyzConverter_;
		std::unique_ptr<RGBConverter> rgbConverter_;
		std::unique_ptr<Model> model_;
		std::unique_ptr<TaskScheduler> taskScheduler_;
		std::vector<Vec3> pixels_;
		std::shared_ptr<ImageUpdater> imageUpdater_;
		std::chrono::high_resolution_clock::time_point start_;

		double FindExposure(const std::vector<Vec3>& xyzColor) const;
		double Clamp(double c) const;
		std::vector<Vec3> Tonemap(const std::vector<Vec3>& xyzColor) const;
		std::string FormatNumber(double n) const;
	};

}

#endif
