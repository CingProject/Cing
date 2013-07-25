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

  Original code based on MemoryDumper by David Hernanded Cerpa
*/

#ifndef _MEMORY_DUMPER_
#define _MEMORY_DUMPER_

#if defined(WIN32)

/**
 * This class generates minidumps when the application crashes due to an unhandled exception.
 * Note: This class is only valid in MS Windows 
 */
class MemoryDumper
{
public:

  /// Core dumps types
  enum DUMP_TYPE {
    CALL_STACK_DUMP,  ///< Dump with call stack data.
    FULL_MEMORY_DUMP  ///< Dump with the process' full memory.
  };

  // Callback in case of crash
  typedef void (*CrashCallback)();

  /**
   * Default constructor. Register the object as an unhandled exception filter.
   * @param type The desired type of dump.
   */ 
  MemoryDumper(DUMP_TYPE type, CrashCallback callback = NULL);
};

// This is to cover the usage of this class in non Windows OS
#else

class MemoryDumper
{
public:
    
    /// Core dumps types
    enum DUMP_TYPE {
        CALL_STACK_DUMP,  ///< Dump with call stack data.
        FULL_MEMORY_DUMP  ///< Dump with the process' full memory.
    };
    
    // Callback in case of crash
    typedef void (*CrashCallback)();

	MemoryDumper(DUMP_TYPE type, CrashCallback callback = NULL) {}
};

#endif // defined(WIN32)


#endif
