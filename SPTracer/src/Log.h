#ifndef SPT_LOG_H
#define SPT_LOG_H

#include <string>

namespace sptracer {

	class Log
	{
	public:
		static void Trace(std::string msg);
		static void Debug(std::string msg);
		static void Info(std::string msg);
		static void Warning(std::string msg);
		static void Error(std::string msg);

	private:
		enum class Level
		{
			Trace = 0,
			Debug = 1,
			Info = 2,
			Warning = 3,
			Error = 4,
		};

		enum class State
		{
			NotInitialized,
			InitializationFailed,
			Initialized
		};

		static const char FileName[];
		static Level _minLogLevel;
		static State _state;

		Log();
		static void Init();
		static std::string FormatTime();
		static std::string FormatLevel(Level logLevel);
		static void Write(Level logLevel, std::string msg);
	};

}

#endif