#pragma once
#include <ostream>

namespace MoonEngine
{
	enum LogLevel
	{
		FATAL_ERROR = 0,
		ERROR       = 1,
		WARN        = 2,
		INFO		= 3,

		VERBOSE     = 99
	};
	class Logger
	{
		public:
			/**
			 * Provide an alternative output for the log.
			 * @param str stream to a logging output.
			 */
			static void ProvideErrorStream(std::ostream * str);
			/**
			 * Set the log level for the application
			 (Default: 1)
			 * @param level the logging level.
			 */
			static void SetLogLevel(LogLevel level);
			/**
			 * Write to the logger.
			 */
			static void Log(LogLevel lv,std::string log);
		private:
			static std::ostream * _logStream;
			static LogLevel     _logLevel;
	};

	inline void LOG(LogLevel lv, std::string log)
	{
		Logger::Log(lv,log);
	}

}