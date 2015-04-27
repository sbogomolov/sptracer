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
		pixels_.resize(static_cast<size_t>(width_ * height_) * 3);

		// wave length parameters
		waveLengthMin_ = 400.0f;
		waveLengthMax_ = 700.0f;
		waveLengthStep_ = 20.0f;

#ifdef _DEBUG
		waveLengthStep_ = 150.0f;
#endif

		waveLengthCount_ = static_cast<size_t>((waveLengthMax_ - waveLengthMin_) / waveLengthStep_) + 1;

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

	float SPTracer::GetWaveLengthMin() const
	{
		return waveLengthMin_;
	}

	float SPTracer::GetWaveLengthMax() const
	{
		return waveLengthMax_;
	}

	float SPTracer::GetWaveLengthStep() const
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

		// add tasks to start sampling
		for (size_t i = 0; i < numThreads_; i++)
		{
			taskScheduler_->AddTask(std::make_unique<TraceTask>(*this));
		}
	}

	void SPTracer::AddSamples(std::vector<Vec3>& color)
	{
		// lock
		std::lock_guard<std::mutex> lock(mutex_);

		// for every pixel
		for (size_t i = 0; i < color.size(); i++)
		{
			pixels_[3 * i] += static_cast<long double>(color[i].x);
			pixels_[3 * i + 1] += static_cast<long double>(color[i].y);
			pixels_[3 * i + 2] += static_cast<long double>(color[i].z);
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

	float SPTracer::FindExposure(const std::vector<Vec3>& xyzColor) const
	{
		float n = static_cast<float>(width_ * height_);

		// Calculate the average intensity. Calculations are based
		// on the CIE Y component, which corresponds to lightness.
		float mean = std::accumulate(xyzColor.begin(), xyzColor.end(), 0.0f,
			[](float a, const Vec3& xyz) { return a + xyz.y; }) / n;

		// Then compute the standard deviation.
		float sqrMean = std::accumulate(xyzColor.begin(), xyzColor.end(), 0.0f,
			[](float a, const Vec3& xyz) { return a + xyz.y * xyz.y; }) / n;

		float variance = sqrMean - mean * mean;

		// The desired 'white' is one standard deviation above average
		return mean + std::sqrt(variance);
	}

	float SPTracer::Clamp(float c) const
	{
		return std::min(1.0f, std::max(0.0f, c));
	}

	std::vector<Vec3> SPTracer::Tonemap(const std::vector<Vec3>& xyzColor) const
	{
		float maxIntensity = FindExposure(xyzColor);
		std::vector<Vec3> rgbColor(xyzColor.size());

		for (size_t i = 0; i < xyzColor.size(); i++)
		{
			Vec3 xyz = xyzColor[i];
			Vec3& rgb = rgbColor[i];

			// Apply exposure correction
			xyz.x = std::log(xyz.x / maxIntensity + 1.0f) / std::log(4.0f);
			xyz.y = std::log(xyz.y / maxIntensity + 1.0f) / std::log(4.0f);
			xyz.z = std::log(xyz.z / maxIntensity + 1.0f) / std::log(4.0f);

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

		// divide XYZ color in pixels on the number of samples
		long double num = static_cast<long double>(completedSamplesCount_ * waveLengthCount_);
		std::vector<Vec3> xyzColor(pixels_.size() / 3);
		for (size_t i = 0; i < xyzColor.size(); i++)
		{
			xyzColor[i].x = static_cast<float>(pixels_[i * 3] / num);
			xyzColor[i].y = static_cast<float>(pixels_[i * 3 + 1] / num);
			xyzColor[i].z = static_cast<float>(pixels_[i * 3 + 2] / num);
		}

		// tonemap XYZ to RGB
		std::vector<Vec3> rgbColor = Tonemap(xyzColor);

		// prepare title
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - start_);
		size_t spp = completedSamplesCount_ * waveLengthCount_;
		float sps = (static_cast<float>(spp * width_ * height_) / 
			(static_cast<float>(duration.count()) / 1000.0f));
		
		std::ostringstream oss;
		oss << FormatNumber(static_cast<float>(spp)) << " SPP, " << FormatNumber(sps) << " SPS" ;

		// call image updater
		imageUpdater_->UpdateImage(rgbColor, oss.str());
	}

	std::string SPTracer::FormatNumber(float n) const
	{
		std::ostringstream oss;
		if (n < 1e3f)
		{
			oss << static_cast<size_t>(n);
		}
		else
		{
			oss << std::setprecision(2) << std::fixed;
			if (n < 1e6f)
			{
				oss << n / 1e3f << "K";
			}
			else if (n <= 1e9f)
			{
				oss << n / 1e6f << "M";
			}
			else
			{
				oss << n / 1e9f << "B";
			}
		}

		return oss.str();
	}

}
