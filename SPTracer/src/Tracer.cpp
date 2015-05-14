#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <thread>
#include "Model/Model.h"
#include "Color/CIE1931.h"
#include "Color/SRGB.h"
#include "Task/TaskScheduler.h"
#include "Task/TraceTask.h"
#include "Camera.h"
#include "ImageUpdater.h"
#include "Ray.h"
#include "Tracer.h"

namespace SPTracer {

	Tracer::Tracer(std::unique_ptr<Model> model, Camera camera,
		unsigned int width, unsigned int height, unsigned int numThreads,
		Spectrum spectrum)
		: model_(std::move(model)), camera_(std::move(camera)),
		  width_(width), height_(height), numThreads_(numThreads),
		  spectrum_(std::move(spectrum))
	{
		// total pixels count
		pixelsCount_ = static_cast<unsigned long>(width_) * height_;

		// create task scheduler that will spawn threads
		taskScheduler_ = std::make_unique<TaskScheduler>(*this, numThreads);

		// prepare array of pixels
		pixels_.resize(pixelsCount_);

		// xyz color converter
		xyzConverter_ = std::make_unique<CIE1931>();

		// rgb color system
		rgbConverter_ = std::make_unique<SRGB>();

		// normalize camera directions
		camera_.n.Normalize();
		camera_.up.Normalize();
	}

	Tracer::~Tracer()
	{
	}

	const Camera& Tracer::GetCamera() const
	{
		return camera_;
	}

	const Model& Tracer::GetModel() const
	{
		return *model_;
	}

	const XYZConverter& Tracer::GetXYZConverter() const
	{
		return *xyzConverter_;
	}

	TaskScheduler& Tracer::GetTaskScheduler() const
	{
		return *taskScheduler_;
	}

	const Spectrum& Tracer::GetSpectrum() const
	{
		return spectrum_;
	}

	unsigned int Tracer::GetWidth() const
	{
		return width_;
	}

	unsigned int Tracer::GetHeight() const
	{
		return height_;
	}

	unsigned long Tracer::GetPixelsCount() const
	{
		return pixelsCount_;
	}

	void Tracer::Run()
	{
		// record start time
		start_ = std::chrono::high_resolution_clock::now();

		// add tasks to start sampling
		for (size_t i = 0; i < numThreads_; i++)
		{
			taskScheduler_->AddTask(std::make_unique<TraceTask>(*this));
		}
	}

	void Tracer::AddSamples(std::vector<Vec3>& color)
	{
		// lock
		std::lock_guard<std::mutex> lock(mutex_);

		// for every pixel
		for (size_t i = 0; i < pixels_.size(); i++)
		{
			PixelData& pd = pixels_[i];
			Vec3& c = color[i];
			
			pd.x += c[0];
			pd.y += c[1];
			pd.z += c[2];
			pd.samples++;
		}

		// increase count of completed samples
		completedPasses_++;

		// update image approximately every 10 seconds
		static const auto updateInterval = std::chrono::seconds(10);
		static auto nextUpdate = std::chrono::steady_clock::now() + updateInterval;
		auto now = std::chrono::steady_clock::now();
		if (now >= nextUpdate)
		{
			UpdateImage();
			nextUpdate = now + updateInterval;
		}
	}

	void Tracer::SetImageUpdater(std::shared_ptr<ImageUpdater> imageUpdater)
	{
		imageUpdater_ = std::move(imageUpdater);
	}

	float Tracer::FindExposure(const std::vector<Vec3>& xyzColor) const
	{
		float n = static_cast<float>(pixelsCount_);

		// Calculate the average intensity. Calculations are based
		// on the CIE Y component, which corresponds to lightness.
		float mean = std::accumulate(xyzColor.begin(), xyzColor.end(), 0.0f,
			[](float sum, const Vec3& xyz) { return sum + xyz[1]; }) / n;

		// Then compute the standard deviation.
		float sqrMean = std::accumulate(xyzColor.begin(), xyzColor.end(), 0.0f,
			[](float sum, const Vec3& xyz) { return sum + xyz[1] * xyz[1]; }) / n;

		float variance = sqrMean - mean * mean;

		// The desired 'white' is one standard deviation above average
		return mean + std::sqrt(variance);
	}

	float Tracer::Clamp(float c) const
	{
		return std::min(1.0f, std::max(0.0f, c));
	}

	std::vector<Vec3> Tracer::Tonemap(const std::vector<Vec3>& xyzColor) const
	{
		float maxIntensity = FindExposure(xyzColor);
		std::vector<Vec3> rgbColor(xyzColor.size());

		for (size_t i = 0; i < xyzColor.size(); i++)
		{
			Vec3 xyz = xyzColor[i];
			Vec3& rgb = rgbColor[i];

			// Apply exposure correction
			xyz[0] = std::log(xyz[0] / maxIntensity + 1.0f) / std::log(4.0f);
			xyz[1] = std::log(xyz[1] / maxIntensity + 1.0f) / std::log(4.0f);
			xyz[2] = std::log(xyz[2] / maxIntensity + 1.0f) / std::log(4.0f);

			// Convert to sRGB.
			rgb = rgbConverter_->GetRGB(xyz);

			// Clamp colours to saturate.
			rgb[0] = Clamp(rgb[0]);
			rgb[1] = Clamp(rgb[1]);
			rgb[2] = Clamp(rgb[2]);
		}

		return rgbColor;
	}

	void Tracer::UpdateImage()
	{
		if (imageUpdater_ == nullptr)
		{
			return;
		}

		// divide XYZ color in pixels on the number of samples
		std::vector<Vec3> xyzColor;
		xyzColor.reserve(pixels_.size());
		for (size_t i = 0; i < pixels_.size(); i++)
		{
			PixelData& pd = pixels_[i];
			xyzColor.push_back(Vec3(
				static_cast<float>(pd.x / pd.samples),
				static_cast<float>(pd.y / pd.samples),
				static_cast<float>(pd.z / pd.samples)
			));
		}

		// tonemap XYZ to RGB
		std::vector<Vec3> rgbColor = Tonemap(xyzColor);

		// samples per pixel
		float spp = static_cast<float>(std::accumulate(pixels_.begin(), pixels_.end(), 0.0,
			[](double sum, const PixelData& pd) { return sum + pd.samples; }) / pixelsCount_);

		// rays per second
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - start_);
		float rps = static_cast<float>(static_cast<double>(completedPasses_) * pixelsCount_ / duration.count() * 1000.0);
		
		std::ostringstream oss;
		oss << "SPP: " << FormatNumber(spp) << "  RPS: " << FormatNumber(rps);

		// call image updater
		imageUpdater_->UpdateImage(rgbColor, oss.str());
	}

	std::string Tracer::FormatNumber(float n) const
	{
		std::ostringstream oss;
		if (n < 1e3f)
		{
			oss << static_cast<unsigned long>(n);
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
