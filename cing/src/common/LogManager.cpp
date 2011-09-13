/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// Precompiled headers
#include "Cing-Precompiled.h"

#include "LogManager.h"
#include "eString.h"
#include "WindowsEventLogger.h"


#include <PTypes/include/ptime.h>

// GUI
#include "gui/GUIManagerCEGUI.h"

// To use the outputdebugstring
#ifdef WIN32
#include <Windows.h>
#endif

namespace Cing
{

// Static member init
std::string LogManager::logFileName = "Cing";

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
LogManager::LogManager():
	m_log			( NULL  ),
	m_logToOutput	( false ),
	m_logToFile		( false ),
	m_bIsValid		( false ),
	m_enabled		( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
LogManager::~LogManager()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Initializes the log system. By default, in debug mode log is active
 * and in release mode is inactive.
 *
 * @param logToOutput if true logs will be directed to the debug output
 * @param logToFile		if true logs will be directed to the log file
 * @note both parameters are independent
 */
void LogManager::init( bool logToOutput /*= true*/, bool logToFile /*= true*/ )
{
	// Store log configuration
	m_logToOutput	= logToOutput;
	m_logToFile		= logToFile;

	// By default just lot error messages to debug output
	m_debugOutputLogLevel = LOG_NORMAL;

	// Get ogre log pointer (just in case)
	m_ogreLog = Ogre::LogManager::getSingleton().getDefaultLog();

	// Add the date to the log file name
	pt::datetime currentTime =  pt::now(false);
	int year, month, day;
	int hour, min, sec, msec;
	pt::decodedate(currentTime, year, month, day);
	pt::decodetime(currentTime, hour, min, sec, msec);

	logFileName += "-" + intToString(year) + intToString(month) + intToString(day) + "_";
	logFileName += intToString(hour) + "-" + intToString(min) + "-" + intToString(sec);
	logFileName += ".log";

	// Create custom log (the default log for Cing from now on, not for Ogre which will output to Ogre.log)
	m_log = Ogre::LogManager::getSingleton().createLog( logFileName, false, logToOutput, !logToFile );

	// Set log level
	setLogLevel(m_debugOutputLogLevel);

	m_bIsValid = true;
	m_enabled  = true;
}

/**
 * @internal
 * @brief Releases the class resources.
 * After this call no method of this object can be called without calling init method again.
 */
void LogManager::end()
{
	m_bIsValid	= false;
	m_enabled	= false;
}

/**
 * @brief Set the output log level. The lower the level, the more verbose Cing will be.
 *
 * @note If you are not debugging an application, is better in terms of performance no to set the LOG_ERROR as log level
 * to avoid having to many log messages
 * @param level Log Level (LOG_ERROR, LOG_NORMAL, or LOG_TRIVIAL);
 */
void LogManager::setLogLevel( LogMessageLevel level )
{
	if ( !m_bIsValid )
	{
		std::cerr << "LogManager::setLogLevel Error: cannot set error level. LogManager may not be initialized yet";
		return;
	}

	Ogre::LoggingLevel loggingLevel = Ogre::LL_NORMAL;
	switch( level )
	{
		// We want all logs (from trivial to errors) to be reported
		case LOG_TRIVIAL:
			loggingLevel = Ogre::LL_BOREME; 
		break;

		// We want normal and critical logs to be reported (but not trivial)
		case LOG_NORMAL:
			loggingLevel = Ogre::LL_NORMAL; 
		break;
	
		// We want only critical logs to be reported
		case LOG_CRITICAL:
			loggingLevel = Ogre::LL_LOW; 
		break;

	}

	// Set level to both Cing and Ogre logs
	m_ogreLog->setLogDetail( loggingLevel );
	m_log->setLogDetail( loggingLevel );
	m_debugOutputLogLevel = level;
}


/**
 * @internal
 * @brief Logs a message. If no LogLevel specified, the default value will be LOG_NORMAL
 *
 * @param level Relevance level of the message to log. It can be: LOG_NORMAL or LOG_ERROR (for critical failures)
 * @param msg		Message to log, with variable parameter format (this is a printf like format)
 */
void LogManager::logMessage( LogMessageLevel level, const char* msg, ... )
{
    if ( !m_bIsValid || !m_enabled )
        return;

	// Extract string parameters
	char			msgFormated[4096];
	va_list		args;
	va_start	(args, msg);
	vsprintf	(msgFormated, msg, args);
	va_end		(args);

	// Log message normally
	if ( m_log && (level >= m_debugOutputLogLevel) )
	{
		m_log->logMessage( msgFormated, (Ogre::LogMessageLevel)level );
	}
	// Log is not ready yet (probably we are initializing the app)
	else if (level >= m_debugOutputLogLevel)
		printf("%s\n", msgFormated);
	

	// Send it to the debug console
	// TODO: decide policy
	if ( level >= m_debugOutputLogLevel )
    {
		GUIManagerCEGUI::getSingleton().getDebugOutput().println( msgFormated );
    }

	// If we are in windows and debug -> log to visual studio output
#if defined(WIN32)
	if ( level >= m_debugOutputLogLevel )
	{
		OutputDebugString( msgFormated );	// In release, only critical messages
		OutputDebugString( "\n" );
	}


	// all windows event logging will be executed directly
	// Windows Event logger
	int windows_log_level = 0;
	WindowsEventLogger* wel = &WindowsEventLogger::getSingleton(); 
	if ( wel && wel->getUseWindowsEventLogger() )
	{
		// convert the ogre log level to windows log level
		switch( level )
		{
			case LOG_CRITICAL:
				windows_log_level = EVENTLOG_ERROR_TYPE;
				break;
			case LOG_NORMAL:
				windows_log_level = EVENTLOG_WARNING_TYPE;
				break;
			default:
				windows_log_level = EVENTLOG_INFORMATION_TYPE;
				break;
		}

		wel->write(msgFormated, windows_log_level);
	}

#endif
}

} // namespace Cing
