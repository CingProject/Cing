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

#ifndef _CingLogManager_h_
#define _CingLogManager_h_

#include "CommonPrereqs.h"
#include "Singleton.h"

// Ogre
#include "OgreLogManager.h"

// Macros for handy use of log system

// Visual studio
#if defined( _MSC_VER )
	#define LOG(x, ...)					Cing::LogManager::getSingleton().logMessage( Cing::LogManager::LOG_NORMAL, x, __VA_ARGS__ )	
	#define LOG_ERROR(x, ...)			Cing::LogManager::getSingleton().logMessage( Cing::LogManager::LOG_ERROR, x, __VA_ARGS__ )
	#define LOG_ERROR_NTIMES(n, x, ...)	static int count = 0; \
										if ( ++count <= n )	  \
											Cing::LogManager::getSingleton().logMessage( Cing::LogManager::LOG_ERROR, x, __VA_ARGS__ )

// GNU
#else
	#define LOG(x, args...)			LogManager::getSingleton().logMessage( LogManager::LOG_NORMAL, x, ## args )
	#define LOG_ERROR(x, args...)	LogManager::getSingleton().logMessage( LogManager::LOG_ERROR, x, ## args )
#endif


namespace Cing
{

/**
 * @internal
 * @brief Manages logs system of the application. Allows to generate log messages to files or debug output.
 *
 * The whole application generates two log files
 * - Ogre.log: Contains the log information of the graphics engine
 * - Cing.log: Contains the log informatio of the rest of the subsystems
 *
 * There are only two kind of log messages (for the sake of simplicity)
 * - LOG_NORMAL: Default log message. It usually means that there is some minor problem, but the application can continue.
 * - LOG_ERROR: It means that there is some critical problem, so the application usually cannot continue executing.
 *
 * By default, the log messages work as follows:
 * - In debug mode: Normal and Critical messages are reported to the log file, and to the debug output console.
 * - In release mode: Only Critical messages are reported to the log file, and to the debug output console.*
 * @code
 *	// Example use:
 * 	LogManager::getSingleton().logMessage( "Normal log message" );
 *	LogManager::getSingleton().logMessage( "Critical log message", LogManager::LOG_ERROR );
 * @endcode
 */
class LogManager: public SingletonStatic< LogManager >
{
public:

	// Available log levels
	enum LogMessageLevel
	{
		LOG_NORMAL	= Ogre::LML_NORMAL,		///< Normal log level. It means that there is some problem, but the application can continue executing
		LOG_ERROR		= Ogre::LML_CRITICAL,	///< Critical log level. It means a failure that makes impossible the continuation of the execution (for a subsistem at least)
		LOG_SILENT												///< No message will be output
	};

	// Singleton needs
	friend class SingletonStatic< LogManager >;

	// Destructor
	~LogManager();

	// Init / Release / Update
	void	init 				( bool logToOutput = true, bool logToFile = true );
	void	end					();

	// Log control
	void	logNormalMsgsToDebugOutput( bool value );
	void	logErrorMsgsToDebugOutput	( bool value );

	// Query  Methods
	bool	isValid			() { return m_bIsValid; }

	// Log
	void	logMessage	( LogMessageLevel level, const char* msg, ... );

	// Constants
	static const std::string logFileName; ///< Name of the log file

private:
	// Private constructor to ensure singleton
	LogManager();

	// Attributes
	Ogre::Log*				m_log;								///< Log used to output messages
	Ogre::Log*				m_ogreLog;						///< Ogre Log, used to output ogre engine messages
	LogMessageLevel		m_debugOutputLogLevel;///< Min log level for a message to be directed to the debug output
	bool							m_logToOutput;				///< If true messages will be directed to output
	bool							m_logToFile;					///< If true messages will be directed to the log file
	bool							m_bIsValid;						///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _LogManager_h_
