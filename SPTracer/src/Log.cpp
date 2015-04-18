#include <cstdio>
#include <ctime>
#include <fstream>
#include <sstream>
#include "Log.h"

namespace sptracer {

	// log file name
	const char Log::FileName[] = "sptracer.log";
	
	// set initial state
	Log::State Log::_state = Log::State::NotInitialized;

	// set min log level
	Log::Level Log::_minLogLevel =
#ifdef _DEBUG
		Level::Trace;
#else
		Level::Info;
#endif

	void Log::Init()
	{
		// try to open log file to check if it exists
		std::ifstream file(FileName);
		if (file)
		{
			// close log file
			file.close();

			// try to delete file
			if (std::remove(FileName))
			{
				// initialization failed
				_state = State::InitializationFailed;
				return;
			}
		}

		// initialization completed succesfully
		_state = State::Initialized;
	}

	std::string Log::FormatTime()
	{
		// get current time
		auto t = std::time(nullptr);

		// buffer for formatted time
		char buf[64];
		
		// format time
		std::strftime(buf, sizeof(buf), "[%d/%m/%Y %H:%M:%S] ", std::localtime(&t));

		// return std::string with formatted time
		return std::string(buf);
	}

	std::string Log::FormatLevel(Level logLevel)
	{
		switch (logLevel)
		{
			case Level::Trace:
				return "TRACE:   ";
			case Level::Debug:
				return "DEBUG:   ";
			case Level::Info:
				return "INFO:    ";
			case Level::Warning:
				return "WARNING: ";
			case Level::Error:
				return "ERROR:   ";
			default:
				return "UNKNOWN: ";
			break;
		}
	}

	void Log::Trace(std::string msg)
	{
		// write log message to file
		Write(Level::Trace, msg);
	}

	void Log::Debug(std::string msg)
	{
		// write log message to file
		Write(Level::Debug, msg);
	}

	void Log::Info(std::string msg)
	{
		// write log message to file
		Write(Level::Info, msg);
	}

	void Log::Warning(std::string msg)
	{
		// write log message to file
		Write(Level::Warning, msg);
	}

	void Log::Error(std::string msg)
	{
		// write log message to file
		Write(Level::Error, msg);
	}

	void Log::Write(Level logLevel, std::string msg)
	{
		// check min log level
		if (logLevel < _minLogLevel)
		{
			return;
		}

		// check if log was initialized
		if (_state == State::NotInitialized)
		{
			Init();
		}

		// return if log initialization failed
		if (_state == State::InitializationFailed)
		{
			return;
		}

		// prepare log message
		std::ostringstream ss;
		ss << FormatTime() << FormatLevel(logLevel) << msg;

		// open log file
		std::ofstream file(FileName, std::ios_base::app);

		// write log message to file
		file << ss.str() << std::endl;
	}

}