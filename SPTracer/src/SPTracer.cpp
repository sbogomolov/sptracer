#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <thread>
#include "Model/MDLAModel.h"
#include "Color/CIE1931.h"
#include "Color/SRGB.h"
#include "Task/TaskScheduler.h"
#include "Task/TraceTask.h"
#include "ImageUpdater.h"
#include "Ray.h"
#include "SPTracer.h"

namespace SPTracer {

	SPTracer::SPTracer(std::string fileName, size_t numThreads, unsigned int width, unsigned int height)
		: width_(width), height_(height), numThreads_(numThreads)
	{
		// create model from file
		model_ = std::make_unique<MDLAModel>(std::move(fileName));

		// create task scheduler that will spawn threads
		taskScheduler_ = std::make_unique<TaskScheduler>(*this, numThreads);

		// prepare array of pixels
		pixels_.resize((size_t)width_ * height_);

		// wave length parameters
		waveLengthMin_ = 400.0;
		waveLengthMax_ = 700.0;
		waveLengthStep_ = 20.0;

#ifdef _DEBUG
		waveLengthStep_ = 150.0;
#endif

		waveLengthCount_ = (size_t)((waveLengthMax_ - waveLengthMin_) / waveLengthStep_) + 1;

		// color converter
		xyzConverter_ = std::make_unique<CIE1931>();

		// color system
		rgbConverter_ = std::make_unique<SRGB>();

	}

	SPTracer::~SPTracer()
	{
	}

	const Model& SPTracer::GetModel() const
	{
		return *model_;
	}

	const XYZConverter& SPTracer::GetXYZConverter() const
	{
		return *xyzConverter_;
	}

	TaskScheduler& SPTracer::GetTaskScheduler() const
	{
		return *taskScheduler_;
	}

	double SPTracer::GetWaveLengthMin() const
	{
		return waveLengthMin_;
	}

	double SPTracer::GetWaveLengthMax() const
	{
		return waveLengthMax_;
	}

	double SPTracer::GetWaveLengthStep() const
	{
		return waveLengthStep_;
	}

	size_t SPTracer::GetWaveLengthCount() const
	{
		return waveLengthCount_;
	}

	size_t SPTracer::GetCompletedSamplesCount() const
	{
		return completedSamplesCount_;
	}

	unsigned int SPTracer::GetWidth() const
	{
		return width_;
	}

	unsigned int SPTracer::GetHeight() const
	{
		return height_;
	}

	void SPTracer::Run()
	{
		// record start time
		start_ = std::chrono::high_resolution_clock::now();

		// sample pixels
		for (size_t i = 0; i < 1000; i++)
		{
			taskScheduler_->AddTask(std::make_unique<TraceTask>(*this));
		}
	}

	void SPTracer::AddSamples(std::vector<Vec3>& color)
	{
		// lock
		std::lock_guard<std::mutex> lock(mutex_);

		// for every pixel
		for (size_t i = 0; i < pixels_.size(); i++)
		{
			pixels_[i] += color[i];
		}

		// increase count of completed samples
		completedSamplesCount_++;

		// update image when all threads return samples
		static size_t samples = 0;
		samples++;
		if (samples == numThreads_)
		{
			// reset count
			samples = 0;

			// upadte image
			UpdateImage();
		}
	}

	void SPTracer::SetImageUpdater(std::shared_ptr<ImageUpdater> imageUpdater)
	{
		imageUpdater_ = std::move(imageUpdater);
	}

	double SPTracer::FindExposure(const std::vector<Vec3>& xyzColor) const
	{
		double n = width_ * height_;

		// Calculate the average intensity. Calculations are based
		// on the CIE Y component, which corresponds to lightness.
		double mean = std::accumulate(xyzColor.begin(), xyzColor.end(), 0.0,
			[](double a, const Vec3& xyz) { return a + xyz.y; }) / n;

		// Then compute the standard deviation.
		double sqrMean = std::accumulate(xyzColor.begin(), xyzColor.end(), 0.0,
			[](double a, const Vec3& xyz) { return a + xyz.y * xyz.y; }) / n;

		double variance = sqrMean - mean * mean;

		// The desired 'white' is one standard deviation above average
		return mean + std::sqrt(variance);
	}

	double SPTracer::Clamp(double c) const
	{
		return std::min(1.0, std::max(0.0, c));
	}

	std::vector<Vec3> SPTracer::Tonemap(const std::vector<Vec3>& xyzColor) const
	{
		double maxIntensity = FindExposure(xyzColor);
		std::vector<Vec3> rgbColor(xyzColor.size());

		for (size_t i = 0; i < xyzColor.size(); i++)
		{
			Vec3 xyz = xyzColor[i]/* / (double)(completedSamplesCount_ * waveLengthCount_)*/;
			Vec3& rgb = rgbColor[i];

			// Apply exposure correction
			xyz.x = std::log(xyz.x / maxIntensity + 1.0) / std::log(4.0);
			xyz.y = std::log(xyz.y / maxIntensity + 1.0) / std::log(4.0);
			xyz.z = std::log(xyz.z / maxIntensity + 1.0) / std::log(4.0);

			// Convert to sRGB.
			rgb = rgbConverter_->GetRGB(xyz);

			// Clamp colours to saturate.
			rgb.x = Clamp(rgb.x);
			rgb.y = Clamp(rgb.y);
			rgb.z = Clamp(rgb.z);
		}

		return rgbColor;
	}

	void SPTracer::UpdateImage()
	{
		if (imageUpdater_ == nullptr)
		{
			return;
		}

		// tonemap XYZ to RGB
		std::vector<Vec3> rgbColor = Tonemap(pixels_);

		// prepare title
		size_t spp = completedSamplesCount_ * waveLengthCount_;
		double sps = ((double)(spp * width_ * height_) / ((double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_).count() / 1000.0));
		
		std::ostringstream oss;
		oss << FormatNumber(spp) << " SPP, " << FormatNumber(sps) << " SPS" ;

		// call image updater
		imageUpdater_->UpdateImage(rgbColor, oss.str());
	}

	std::string SPTracer::FormatNumber(double n) const
	{
		std::ostringstream oss;
		if (n < 1e3)
		{
			oss << (size_t)n;
		}
		else
		{
			oss << std::setprecision(2) << std::fixed;
			if (n < 1e6)
			{
				oss << n / 1e3 << "K";
			}
			else if (n <= 1e9)
			{
				oss << n / 1e6 << "M";
			}
			else
			{
				oss << n / 1e9 << "B";
			}
		}

		return oss.str();
	}

}
