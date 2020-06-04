#include <Windows.h>
#include "LogWriter.h"
#include "log4cplus/logger.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/asyncappender.h"
#include "log4cplus/helpers/appenderattachableimpl.h"
#include "log4cplus/spi/loggingevent.h"
#include "log4cplus/layout.h"
#include <codecvt>
#include <shared_mutex>

using namespace log4cplus;
using namespace helpers;
using namespace spi;
#define LOGSIZE         (2*1024)

static bool enable_debug_log = false;
void load_log_config(long& log_size, int& log_number, bool& async_log)
{
	static constexpr char const* path = R"(C:\ProgramData\SimplyLive.TV\Vibox\Backend\LogConfig.ini)";
	log_size = GetPrivateProfileIntA("Log", "size_per_file_MB", 20, path);
	log_size *= (1024 * 1024);
	log_number = GetPrivateProfileIntA("Log", "file_num", 5, path);
	enable_debug_log = GetPrivateProfileIntA("Log", "EnableDebug", false, path);
	async_log = GetPrivateProfileIntA("Log", "Async", true, path);
}

void get_appender(const wchar_t* logPath, SharedAppenderPtr& appenderPtr)
{
	static std::map<std::wstring, SharedAppenderPtr> append_list;
	static std::shared_mutex m_lock;

	{//here can be Concurrency
		std::shared_lock<std::shared_mutex> lock(m_lock);
		const auto appender = append_list.find(logPath);
		if (appender != append_list.end())
		{
			appenderPtr = appender->second;
			return;
		}
	}

	std::lock_guard<std::shared_mutex> lock(m_lock);
	const auto appender = append_list.find(logPath);
	if (appender != append_list.end())
	{
		appenderPtr = appender->second;
		return;
	}

	static long  log_size = 0;
	static int   log_number = 0;
	static bool  async_log = true;
	static std::once_flag flag;
	std::call_once(flag, [&]() {load_log_config(log_size, log_number, async_log); });

	RollingFileAppender* rollingFileAppender = new RollingFileAppender(logPath, log_size, log_number, true, true);
	rollingFileAppender->setName(logPath);
	tstring pattern = LOG4CPLUS_TEXT("%D{%y/%m/%d %H:%M:%S.%q} [%-5t][%-5p]- %m%n");
	rollingFileAppender->setLayout(std::unique_ptr<Layout>(std::make_unique<PatternLayout>(pattern)));
	rollingFileAppender->imbue(std::locale(rollingFileAppender->getloc(),
		new std::codecvt_utf8<tchar, 0x10FFFF, static_cast<std::codecvt_mode>(std::consume_header | std::little_endian)>));

	SharedAppenderPtr smart_ptr(rollingFileAppender);
	if (async_log)
	{
		appenderPtr = new AsyncAppender(smart_ptr, 100);
		append_list.emplace(logPath, appenderPtr);
	}
	else
	{
		append_list.emplace(logPath, smart_ptr);
		appenderPtr = smart_ptr;
	}
}

void internal_writer(const wchar_t* logPath, LOGLEVEL logLevel, std::wstring_view log)
{
	if (logPath == nullptr)
		return;

	SharedAppenderPtr appenderPtr;
	get_appender(logPath, appenderPtr);
	if (appenderPtr == nullptr)
		return;

	const InternalLoggingEvent event(Logger::getInstance(LOG4CPLUS_TEXT("SimplyLive")).getName(), static_cast<int>(logLevel), log, nullptr, 0);
	appenderPtr->doAppend(event);
}

void WriteLogW(const wchar_t* logPath, LOGLEVEL logLevel, const wchar_t* fmt, ...)
{
	if (logLevel == LOGLEVEL::Debug)
	{
		if (!enable_debug_log)
			return;
	}

	TCHAR szBuffer[LOGSIZE];
	va_list marker;
	va_start(marker, fmt);
	const int ret = _vsntprintf_s(szBuffer, std::size(szBuffer), _TRUNCATE, fmt, marker);
	va_end(marker);

	if (ret < 0)
		return;

	internal_writer(logPath, logLevel, szBuffer);
}

void WriteLog(const wchar_t* logPath, LOGLEVEL logLevel, std::wstring_view fmt)
{
	if (logLevel == LOGLEVEL::Debug)
	{
		if (!enable_debug_log)
			return;
	}
	internal_writer(logPath, logLevel, fmt);
}

void InitializeLogger()
{
	initialize();
}

void WriteLogA(const wchar_t* logPath, LOGLEVEL logLevel, const char* fmt, ...)
{
	if (logLevel == LOGLEVEL::Debug)
	{
		if (!enable_debug_log)
			return;
	}

	va_list marker;
	char szBuffer[LOGSIZE / 2] = { 0 };

	va_start(marker, fmt);
	int ret = _vsnprintf_s(szBuffer, std::size(szBuffer), _TRUNCATE, fmt, marker);
	va_end(marker);

	if (ret < 0)
		return;

	wchar_t wLogBuf[LOGSIZE];
	size_t size = 0;
	if (mbstowcs_s(&size, wLogBuf, szBuffer, strlen(szBuffer)) == 0)
		WriteLog(logPath, logLevel, std::wstring_view(wLogBuf));
}

void WriteLog(const wchar_t* logPath, LOGLEVEL logLevel, std::string_view fmt)
{
	if (logLevel == LOGLEVEL::Debug)
	{
		if (!enable_debug_log)
			return;
	}
	wchar_t wLogBuf[LOGSIZE];
	size_t size = 0;
	if (mbstowcs_s(&size, wLogBuf, fmt.data(), fmt.size()) == 0)
		WriteLog(logPath, logLevel, std::wstring_view(wLogBuf));
}

void ShutdownLogger()
{
	deinitialize();
}

std::string SafeA(char* buf)
{
	if (buf == nullptr)
		return "";

	std::string s1(buf);
	std::string::size_type pos = 0;
	std::string_view toPlace("%%");
	std::string_view toFind("%");
	while ((pos = s1.find(toFind, pos)) != std::string::npos)
	{
		s1.replace(pos, 1, toPlace);
		pos += 2;
	}
	return s1;
}

std::string SafeA(const std::string& buf)
{
	if (buf.empty())
		return "";

	std::string s1(buf);
	std::string::size_type pos = 0;
	std::string_view toPlace("%%");
	std::string_view toFind("%");
	while ((pos = s1.find(toFind, pos)) != std::string::npos)
	{
		s1.replace(pos, 1, toPlace);
		pos += 2;
	}
	return s1;
}

std::wstring SafeW(wchar_t* buf)
{
	if (buf == nullptr)
		return L"";

	std::wstring s1(buf);
	std::wstring::size_type pos = 0;
	std::wstring_view toPlace(L"%%");
	std::wstring_view toFind(L"%");
	while ((pos = s1.find(toFind, pos)) != std::wstring::npos)
	{
		s1.replace(pos, 1, toPlace);
		pos += 2;
	}
	return s1;
}

std::wstring SafeW(const std::wstring& buf)
{
	if (buf.empty())
		return L"";

	std::wstring s1(buf);
	std::wstring::size_type pos = 0;
	std::wstring_view toPlace(L"%%");
	std::wstring_view toFind(L"%");
	while ((pos = s1.find(toFind, pos)) != std::wstring::npos)
	{
		s1.replace(pos, 1, toPlace);
		pos += 2;
	}
	return s1;
}

LoggerInitializer::LoggerInitializer()
{
	initialize();
}

LoggerInitializer::~LoggerInitializer()
{
	deinitialize();
}