#pragma once
#include <ostream>
namespace MoonEngine
{
    enum LogLevel
    {
        NONE = -1,
        FATAL_ERROR = 0,
        ERROR = 1,
        WARN = 2,
        GAME = 3,
        INFO = 4,

        VERBOSE = 99
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
        static void Log(LogLevel lv, std::string log, std::string file, int line);

    private:
        static std::ostream * _logStream;
        static LogLevel _logLevel;
    };

    inline void LOG(LogLevel lv, std::string log, std::string file = "", int line = -1)
    {
        Logger::Log(lv, log, file, line);
    }
}