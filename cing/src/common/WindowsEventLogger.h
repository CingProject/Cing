#ifndef _WINDOWS_EVENT_LOGGER
#define _WINDOWS_EVENT_LOGGER

#include "Cing-Precompiled.h"
#include "common/Singleton.h"
#include <windows.h>
#include "XEventLog.h"

namespace Cing {
	class WindowsEventLogger : public Cing::Singleton<WindowsEventLogger>
	{

	public:

		WindowsEventLogger();
		~WindowsEventLogger();

		void setup(int logLevel, bool useWindowsEventLogger, std::string logName);
		void exit();
		void write(char* msg, int level);
		int getLogLevel() { return m_logLevel; }
		bool getUseWindowsEventLogger() { return m_useWindowsEventLogger; }

	protected:

		CXEventLog* m_CXEventLog;
		int m_logLevel;
		bool m_useWindowsEventLogger;

	};

}

#endif
