#include <cstdio>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include "Log.h"

namespace SPTracer {

	// log file name
	const std::string Log::FileName = "sptracer.log";
	
	// set initial state
	Log::State Log::state_ = Log::State::NotInitialized;

	void Log::Init()
	{
		// try to open log file to check if it exists
		std::ifstream file(FileName);
		if (file)
		{
			// close log file
			file.close();

			// try to delete file
			if (std::remove(FileName.c_str()))
			{
				// initialization failed
				state_ = State::InitializationFailed;
				return;
			}
		}

		// initialization completed succesfully
		state_ = State::Initialized;
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

	std::string Log::FormatMessage(std::string msg, size_t prefixLen)
	{
		// replace all "\r\n" with "\n"
		size_t pos = 0;
		while ((pos = msg.find("\r\n", pos)) != std::string::npos)
		{
			msg = msg.replace(pos, 2, "\n");
		}

		// replace all "\r" with "\n"
		pos = 0;
		while ((pos = msg.find('\r', pos)) != std::string::npos)
		{
			msg = msg.replace(pos, 1, "\n");
		}

		// split on to lines
		pos = 0;
		std::string::size_type start = 0;
		std::vector<std::string> lines;
		while ((pos = msg.find('\n', start)) != std::string::npos)
		{
			lines.push_back(msg.substr(start, pos - start));
			start = pos + 1;
			if (start == msg.length())
			{
				break;
			}
		}

		// add last line
		lines.push_back(msg.substr(start, msg.length() - start));

		// add first line
		std::ostringstream formatted;
		formatted << lines[0] << std::endl;

		// just one line, nothing else to do
		if (lines.size() == 1)
		{
			return formatted.str();
		}

		// generate prefix
		std::ostringstream oss;
		for (size_t i = 0; i < prefixLen; i++)
		{
			oss << " ";
		}

		auto prefix = oss.str();

		// format message
		for (size_t i = 1; i < lines.size(); i++)
		{
			formatted << prefix << lines[i] << std::endl;
		}

		return formatted.str();
	}

	void Log::Trace(std::string msg)
	{
#ifdef _DEBUG
		// write log message to file
		Write(Level::Trace, msg);
#endif
	}

	void Log::Debug(std::string msg)
	{
#ifdef _DEBUG
		// write log message to file
		Write(Level::Debug, msg);
#endif
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
		// check if log was initialized
		if (state_ == State::NotInitialized)
		{
			Init();
		}

		// return if log initialization failed
		if (state_ == State::InitializationFailed)
		{
			return;
		}

		// format prefix
		auto time = FormatTime();

		// format log level
		auto level = FormatLevel(logLevel);

		// format log message
		msg = FormatMessage(msg, time.length() + level.length());

		// prepare log message
		std::ostringstream ss;
		ss << time << level << msg;

		// open log file
		std::ofstream file(FileName, std::ios_base::app);

		// write log message to file
		file << ss.str();
	}

}