#include "stdafx.h"
#include "App.h"
#include "Camera.h"
#include "Exception.h"
#include "Log.h"
#include "StringUtil.h"
#include "Tracer.h"
#include "Vec3.h"
#include "Window.h"
#include "Model/MDLAModel.h"
#include "Model/OBJModel.h"

App::App(std::string configFile)
{
	// read config
	Config config = ReadConfig(configFile);

	// window title
#ifdef _WIN64
	std::string title = "SPTracer (64-bit) - [ " + config.modelFile + " ]";
#else
	std::string title = "SPTracer (32-bit) - [ " + config.modelFile + " ]";
#endif

	// create window
	window_ = std::make_unique<Window>(config.width, config.height, title.c_str());

	try
	{
		std::unique_ptr<SPTracer::Model> model;
		SPTracer::Camera camera{};
		
		if (config.modelType == Config::ModelType::MDLA)
		{
			// load MDLA model
			model = std::make_unique<SPTracer::MDLAModel>(config.modelFile, config.spectrum, camera);			
		}
		else if (config.modelType == Config::ModelType::OBJ)
		{
			// load OBJ model
			model = std::make_unique<SPTracer::OBJModel>(config.modelFile, config.spectrum);
			
			// camera data must be present in config file
			if (!config.cameraLoaded)
			{
				throw std::exception("Camera data was not found in config file");
			}
		}

		// camera in configuration file has higher priority
		if (config.cameraLoaded)
		{
			camera = std::move(config.camera);
		}

		// create tracer
		tracer_ = std::make_unique<SPTracer::Tracer>(std::move(model), std::move(camera),
			config.width, config.height, config.numThreads,
			config.spectrum);

		// assign image updater
		tracer_->SetImageUpdater(window_->imageUpdater());
	}
	catch (SPTracer::Exception e)
	{
		// show message box with error
		MessageBoxA(window_->hwnd(), e.what(), "Error", MB_ICONERROR | MB_OK);
		std::terminate();
		return;
	}

	// application is initialized
	initialized_ = true;
}

App::~App()
{
}

int App::Run()
{
	// check if application is initialized
	if (!initialized_)
	{
		std::string s = "Trying to run not initialized application";
		SPTracer::Log::Error(s);
		throw std::exception(s.c_str());
	}

	// run tracer
	tracer_->Run();

	// windows message loop
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

bool App::IsInitialized() const
{
	return initialized_;
}

Config App::ReadConfig(std::string configFile) const
{
	// prepare config
	Config config{};

	// open config file
	std::ifstream file(configFile);
	if (!file.is_open())
	{
		std::string msg = "Cannot open configuration file: " + configFile;
		SPTracer::Log::Error(msg);
		throw std::exception(msg.c_str());
	}
	
	try
	{
		// read file line by line
		std::string line;
		while (std::getline(file, line))
		{
			// store original line
			std::string originalLine(line);

			// skip all white spaces and tab characters
			SPTracer::StringUtil::TrimBegin(line, " \t");

			// trim comment if any
			size_t pos = line.find('#');
			if (pos != line.npos)
			{
				line.erase(pos);
			}

			// skip empty line
			if (line.length() == 0)
			{
				continue;
			}

			// split on parameter and value
			pos = line.find('=');
			if (pos == originalLine.npos)
			{
				throw std::exception(("Error in configuration file: " + originalLine).c_str());
			}

			std::string parameter = line.substr(0, pos);
			std::string value = line.substr(pos + 1);

			// trim spaces
			SPTracer::StringUtil::Trim(parameter, " \t");
			SPTracer::StringUtil::Trim(value, " \t");

			// check that parameter and value are not an empty string
			if ((parameter.length() == 0) || (value.length() == 0))
			{
				throw std::exception(("Error in configuration file: " + originalLine).c_str());
			}

			// convert parameter name to lower
			SPTracer::StringUtil::ToLower(parameter);

			if (parameter == "modeltype")
			{
				// model type
				// convert value to lower
				SPTracer::StringUtil::ToLower(value);
				if (value == "mdla")
				{
					// MDLA model type
					config.modelType = Config::ModelType::MDLA;
				}
				else if (value == "obj")
				{
					// OBJ model type
					config.modelType = Config::ModelType::OBJ;
				}
				else
				{
					// unknown model type
					throw std::exception(("Error in configuration file: Unknown model type: " + originalLine).c_str());
				}
			}
			else if (parameter == "modelfile")
			{
				// model file
				config.modelFile = value;
			}
			else if (parameter == "cameraname")
			{
				// model file
				config.cameraLoaded = true;
				config.camera.name = value;
			}
			else if (parameter == "cameraeyepoint")
			{
				// camera eye point
				config.cameraLoaded = true;
				std::vector<float> values = SPTracer::StringUtil::GetFloatArray(value, 3, ';');
				config.camera.p = SPTracer::Vec3(
					values[0],
					values[1],
					values[2]
				);
			}
			else if (parameter == "cameraviewdirection")
			{
				// camera view direction
				config.cameraLoaded = true;
				std::vector<float> values = SPTracer::StringUtil::GetFloatArray(value, 3, ';');
				config.camera.n = SPTracer::Vec3(
					values[0],
					values[1],
					values[2]
				);
			}
			else if (parameter == "cameraupdirection")
			{
				// camera up direction
				config.cameraLoaded = true;
				std::vector<float> values = SPTracer::StringUtil::GetFloatArray(value, 3, ';');
				config.camera.up = SPTracer::Vec3(
					values[0],
					values[1],
					values[2]
				);
			}
			else if (parameter == "camerafocaldistance")
			{
				// camera focal distance
				config.cameraLoaded = true;
				config.camera.f = SPTracer::StringUtil::GetFloat(value);
			}
			else if (parameter == "cameraimagewidth")
			{
				// camera image width
				config.cameraLoaded = true;
				config.camera.iw = SPTracer::StringUtil::GetFloat(value);
			}
			else if (parameter == "cameraimageheight")
			{
				// camera image height
				config.cameraLoaded = true;
				config.camera.ih = SPTracer::StringUtil::GetFloat(value);
			}
			else if (parameter == "cameraimagecenter")
			{
				// camera image center
				config.cameraLoaded = true;
				std::vector<float> values = SPTracer::StringUtil::GetFloatArray(value, 2, ';');
				config.camera.icx = values[0];
				config.camera.icy = values[1];
			}
			else if (parameter == "width")
			{
				// width
				config.width = (unsigned int)SPTracer::StringUtil::GetInt(value);
			}
			else if (parameter == "height")
			{
				// height
				config.height = (unsigned int)SPTracer::StringUtil::GetInt(value);
			}
			else if (parameter == "numthreads")
			{
				// number of threads
				config.numThreads = (unsigned int)SPTracer::StringUtil::GetInt(value);
			}
			else if (parameter == "wavelengthmin")
			{
				// wave length minimum
				config.spectrum.min = SPTracer::StringUtil::GetFloat(value);
			}
			else if (parameter == "wavelengthmax")
			{
				// wave length maximum
				config.spectrum.max = SPTracer::StringUtil::GetFloat(value);
			}
			else if (parameter == "wavelengthstep")
			{
				// wave length step
				config.spectrum.step = SPTracer::StringUtil::GetFloat(value);
			}
		}
	}
	catch (const std::exception& e)
	{
		std::string msg = "Error in configuration file: " + std::string(e.what());
		SPTracer::Log::Error(msg);
		throw;
	}

	// compute spectrum wave lengths count
	config.spectrum.count = static_cast<unsigned int>((config.spectrum.max - config.spectrum.min) / config.spectrum.step) + 1;

	// precompute wave length
	config.spectrum.values = std::vector<float>(config.spectrum.count);
	for (size_t i = 0; i < config.spectrum.count; i++)
	{
		config.spectrum.values[i] = config.spectrum.min + config.spectrum.step * static_cast<float>(i);
	}

	return config;
}
