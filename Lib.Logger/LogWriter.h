//Wrapper based on https://github.com/log4cplus/log4cplus

#pragma once
#include <string>
#include <tchar.h>
#include "../Lib.FMT/format.h"

#define SAFEA(str) (SafeA(str).c_str())
#define SAFEW(str) (SafeW(str).c_str())

enum class LOGLEVEL
{
	Debug = 10000,
	Info = 20000,
	Warn = 30000,
	Error = 40000,
	Fatal = 50000,
};

class LoggerInitializer
{
public:
	LoggerInitializer();
	~LoggerInitializer();

	LoggerInitializer(LoggerInitializer const &) = delete;
	LoggerInitializer(LoggerInitializer &&) = delete;
	LoggerInitializer & operator = (LoggerInitializer const &) = delete;
	LoggerInitializer & operator = (LoggerInitializer &&) = delete;
};

//always use InitializeLogger as the first thing in main();
void InitializeLogger();

void WriteLogA(const wchar_t* logPath, LOGLEVEL logLevel, const char* fmt, ...);
void WriteLogW(const wchar_t* logPath, LOGLEVEL logLevel, const wchar_t* fmt, ...);

void WriteLog(const wchar_t* logPath, LOGLEVEL logLevel, std::string_view fmt);
void WriteLog(const wchar_t* logPath, LOGLEVEL logLevel, std::wstring_view fmt);

template<typename T, typename... Args>
void WriteLog(const wchar_t* logPath, LOGLEVEL logLevel, const T* fmt, Args&& ... args)
{
	WriteLog(logPath, logLevel, fmt::format(fmt, std::forward<Args>(args)...));
}

//ensures that logger shuts down before the execution leaves the main() function
void ShutdownLogger();

std::string SafeA(char* buf);
std::string SafeA(const std::string& buf);
std::wstring SafeW(wchar_t* buf);
std::wstring SafeW(const std::wstring& buf);

#define LOG_currentThreadID {SetThreadDescription(GetCurrentThread(), __FUNCTIONW__);WriteLog(L"C:\\Logs\\test_NDI\\ThreadInfo.log",LOGLEVEL::Info,"{0} @Line:{1:5d} file:{2}",__FUNCTION__,__LINE__,__FILE__);}