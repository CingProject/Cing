/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

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

#include "LogManager.h"

// To use the outputdebugstring
#ifdef WIN32
#include <Windows.h>
#endif


namespace Common
{

// Static member init
const std::string LogManager::logFileName = "Vision.log";

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
LogManager::LogManager():
	m_log					( NULL  ),
	m_logToOutput	( false ),
	m_logToFile		( false ),
	m_bIsValid		( false )
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
void LogManager::init( bool logToOutput /*= true*/, bool logToFile /*= false*/ )
{
	// Store log configuration
	m_logToOutput = logToOutput;
	m_logToFile		= logToFile;

	// Get ogre log pointer (just in case)
	m_ogreLog = Ogre::LogManager::getSingleton().getDefaultLog();

	// Create custom log (the default log from now on)
	m_log = Ogre::LogManager::getSingleton().createLog( logFileName, false, logToOutput, !logToFile );

	// Set log level depending on release/debug configuration
#if defined(_DEBUG)
	m_ogreLog->setLogDetail( Ogre::LL_NORMAL );
	m_log->setLogDetail( Ogre::LL_NORMAL );
#else
	m_ogreLog->setLogDetail( Ogre::LL_LOW );
	m_log->setLogDetail( Ogre::LL_LOW );
#endif

	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void LogManager::end()
{
	m_bIsValid = false;
}

/**
 * @internal 
 * @brief Logs a message. If no LogLevel specified, the default value will be LOG_NORMAL
 *
 * @param msg		Message to log
 * @param level Relevance level of the message to log. It can be: LOG_NORMAL or LOG_CRITICAL (for critical failures)
 */
void LogManager::logMessage( const std::string& msg, LogMessageLevel level /*= LOG_NORMAL*/ )
{
	// Log message normally
	m_log->logMessage( msg, (Ogre::LogMessageLevel)level );

	// If we are in windows and debug -> log to visual studio output
#if defined(WIN32) && defined(_DEBUG)
		OutputDebugString( msg.c_str() );	// In debug, output everything
		OutputDebugString( "\n" );
#elif defined(WIN32)
	if ( level == LOG_CRITICAL )
	{
		OutputDebugString( msg.c_str() );	// In release, only critical messages
		OutputDebugString( "\n" );
	}
#endif
}

} // namespace Common