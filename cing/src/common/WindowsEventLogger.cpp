#include "Cing-Precompiled.h"
#include "WindowsEventLogger.h"
#include "XEventLog.h"

namespace Cing {
	WindowsEventLogger::WindowsEventLogger()
	{
		m_CXEventLog = NULL;
	}

	WindowsEventLogger::~WindowsEventLogger()
	{
		exit();
	}

	void WindowsEventLogger::setup(int logLevel, bool useWindowsEventLogger, std::string logName) 
	{
		m_logLevel = logLevel;
		m_useWindowsEventLogger = useWindowsEventLogger;

		if ( useWindowsEventLogger )
			m_CXEventLog = new CXEventLog(logName.c_str());//, "."); // framework\\bin\\XEventMessage.dll");
	}

	void WindowsEventLogger::exit()
	{
		delete m_CXEventLog;
		m_CXEventLog = NULL;
	}

	void WindowsEventLogger::write(char* msg, int level)
	{
		if (
			(level == EVENTLOG_INFORMATION_TYPE && m_logLevel >= 3) || // LL_BOREME = 3
			(level == EVENTLOG_WARNING_TYPE && m_logLevel >= 2) || // LL_NORMAL = 2
			(level == EVENTLOG_ERROR_TYPE && m_logLevel >= 1) // LL_LOW = 1
			)
		{
			m_CXEventLog->Write(level, msg);
		}
	}
}