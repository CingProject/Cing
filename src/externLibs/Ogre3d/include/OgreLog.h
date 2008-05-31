/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/

#ifndef __Log_H__
#define __Log_H__

#include "OgrePrerequisites.h"

namespace Ogre {

    // LogMessageLevel + LoggingLevel > OGRE_LOG_THRESHOLD = message logged
    #define OGRE_LOG_THRESHOLD 4

    /** The level of detail to which the log will go into.
    */
    enum LoggingLevel
    {
        LL_LOW = 1,
        LL_NORMAL = 2,
        LL_BOREME = 3
    };

    /** The importance of a logged message.
    */
    enum LogMessageLevel
    {
        LML_TRIVIAL = 1,
        LML_NORMAL = 2,
        LML_CRITICAL = 3
    };

    /** @remarks Pure Abstract class, derive this class and register to the Log to listen to log messages */
    class LogListener
    {
	public:
        virtual ~LogListener() {};

        /**
        @remarks
            This is called whenever the log recieves a message and is about to write it out
        @param message
            The message to be logged
        @param lml
            The message level the log is using
        @param maskDebug
            If we are printing to the console or not
        @param logName
            the name of this log (so you can have several listeners for different logs, and identify them)
        */
        virtual void messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName ) = 0;
    };

    /**
    @remarks
         Log class for writing debug/log data to files.
    @note
        <br>Should not be used directly, but trough the LogManager class.
    */
    class _OgreExport Log
    {
    protected:
        std::ofstream	mfpLog;
        LoggingLevel	mLogLevel;
        bool			mDebugOut;
        bool			mSuppressFile;
        String			mLogName;

        typedef std::vector<LogListener*> mtLogListener;
        mtLogListener mListeners;

    public:
		OGRE_AUTO_MUTEX // public to allow external locking
        /**
        @remarks
            Usual constructor - called by LogManager.
        */
        Log( const String& name, bool debugOutput = true, bool suppressFileOutput = false);

        /**
        @remarks
        Default destructor.
        */
        ~Log();

		/// Return the name of the log
		const String& getName() const { return mLogName; }
		/// Get whether debug output is enabled for this log
		bool isDebugOutputEnabled() const { return mDebugOut; }
		/// Get whether file output is suppressed for this log
		bool isFileOutputSuppressed() const { return mSuppressFile; }

        /** Log a message to the debugger and to log file (the default is
            "<code>OGRE.log</code>"),
        */
        void logMessage( const String& message, LogMessageLevel lml = LML_NORMAL, bool maskDebug = false );

        /**
        @remarks
            Enable or disable outputting log messages to the debugger.
        */
        void setDebugOutputEnabled(bool debugOutput);
		/**
        @remarks
            Sets the level of the log detail.
        */
        void setLogDetail(LoggingLevel ll);
		/** Gets the level of the log detail.
		*/
		LoggingLevel getLogDetail() const { return mLogLevel; }
        /**
        @remarks
            Register a listener to this log
        @param
            A valid listener derived class
        */
        void addListener(LogListener* listener);

        /**
        @remarks
            Unregister a listener from this log
        @param
            A valid listener derived class
        */
        void removeListener(LogListener* listener);
    };
}

#endif
