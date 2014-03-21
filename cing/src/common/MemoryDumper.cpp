/*
  Copyright (c) 2006-2010 Julio Obelleiro and Jorge Cano
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc
 */
// Precompiled headers
#include "Cing-Precompiled.h"

#if defined(WIN32)

#include <string.h>
#include <sstream>
#include <iostream>
#include <tchar.h>

#define _WINSOCKAPI_
#include "windows.h"
#include "dbghelp.h"


#include "MemoryDumper.h"

/*---------------------------------------------------------------------------------------------------------*/

/// Variable used by TopLevelFilter that determines the type of dump
static MemoryDumper::DUMP_TYPE		dumpType = MemoryDumper::CALL_STACK_DUMP;
static MemoryDumper::CrashCallback	callback = NULL;

/*---------------------------------------------------------------------------------------------------------*/

/**
 * The function to be registered as the unhandled exception filter.
 * @param exceptionInfo A pointer to the struct with the exception info.
 * @return The return value needed by the operating system depending on what is done with the exception.
 */
LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *exceptionInfo)
{
  // The type of the MiniDumpWriteDump function.
  typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)( HANDLE, DWORD, HANDLE, MINIDUMP_TYPE,
                                            CONST PMINIDUMP_EXCEPTION_INFORMATION,
                                            CONST PMINIDUMP_USER_STREAM_INFORMATION,
                                            CONST PMINIDUMP_CALLBACK_INFORMATION);

  LONG retval = EXCEPTION_CONTINUE_SEARCH;
  HMODULE hDll = NULL;            // dll handle
  std::ostringstream outputText;

  // firstly see if dbghelp.dll is around and has the function we need
  // look next to the EXE first, as the one in System32 might be old 
  // (e.g. Windows 2000)

  //char szDbgHelpPath[_MAX_PATH]; // Path of dbghelp.dll
  //if (GetModuleFileName( NULL, szDbgHelpPath, _MAX_PATH ))
  //{
  //  char *pSlash = strrchr( szDbgHelpPath, '\\' );
  //  if (pSlash)
  //  {
  //    strcpy( pSlash+1, "DBGHELP.DLL" );
  //    hDll = ::LoadLibrary( szDbgHelpPath );
  //  }
  //}


  // No special dll => use the default LoadLibrary.
  if (hDll==NULL)
    hDll = LoadLibrary( "DBGHELP.DLL" ); // load any version we can

  if (hDll)
  {
    MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)GetProcAddress(hDll, "MiniDumpWriteDump");
    if (pDump)
    {
      std::string dumpPath;     // Path where the dump will be written
      char appPath[_MAX_PATH];  // Application's path

      if (GetModuleFileName(NULL, appPath, _MAX_PATH) != 0) {
        dumpPath = appPath;
        std::string::size_type dotPos = dumpPath.find_last_of('.');

        if (dotPos != std::string::npos)
          dumpPath.replace(dotPos + 1, std::string::npos, "dmp");
        else
          dumpPath.insert(dumpPath.size(), ".dmp");
      }
      else
        dumpPath = "c:\\Dump.dmp"; // An error ocurred while retreiving the application path, store the dump in root directory.

      // create the file
      HANDLE hFile = CreateFile(dumpPath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

      if (hFile != INVALID_HANDLE_VALUE)
      {
        _MINIDUMP_EXCEPTION_INFORMATION ExInfo;

        ExInfo.ThreadId = GetCurrentThreadId();
        ExInfo.ExceptionPointers = exceptionInfo;
        ExInfo.ClientPointers = NULL;

        // Determine the dump type
        MINIDUMP_TYPE type = MiniDumpNormal;
        if (dumpType == MemoryDumper::FULL_MEMORY_DUMP)
          type = MiniDumpWithFullMemory;

        // write the dump
        BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, type, &ExInfo, NULL, NULL);
        if (bOK)
        {
			outputText << "App Crashed: Dump file saved to " << dumpPath;
			retval = EXCEPTION_EXECUTE_HANDLER;
        }
        else
          outputText << "Failed to save dump file to " << dumpPath << " (error " << GetLastError() << ").";

        CloseHandle(hFile);
      }
      else
        outputText << "Failed to create dump file " << dumpPath << " (error " << GetLastError() << ").";
    }
    else
      outputText << "DBGHELP.DLL too old.";
  }
  else
    outputText << "DBGHELP.DLL not found.";

  if (!outputText.str().empty())
    printf( "%s", outputText.str().c_str());

  // Call the registered crash callback
  if ( callback )
	 (*callback)();

  return retval;
}


/*---------------------------------------------------------------------------------------------------------*/
// This is needed to fix changes in how CRT works from MSVS 2005 on
// More info: http://blog.kalmbachnet.de/?postid=75
#ifndef _M_IX86
	#error "The following code only works for x86!"
#endif

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return NULL;	
}


BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL) return FALSE;
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if(pOrgEntry == NULL) return FALSE;
	unsigned char newJump[ 100 ];
	DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;
	dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far
	void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
	DWORD dwNewEntryAddr = (DWORD) pNewFunc;
	DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

	newJump[ 0 ] = 0xE9;  // JMP absolute
	memcpy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));
	SIZE_T bytesWritten;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
	  pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
	return bRet;
}


/*---------------------------------------------------------------------------------------------------------*/

MemoryDumper::MemoryDumper(DUMP_TYPE type, CrashCallback _callback /* = NULL*/ )
{
  // Store the crash callback and dump type
  dumpType = type;
  callback = _callback;

  // Register the top level filter. Only in Debug and VTune configurations.
  SetUnhandledExceptionFilter( TopLevelFilter );

  // This is needed to fix changes in how CRT works from MSVS 2005 on
  // More info: http://blog.kalmbachnet.de/?postid=75
  PreventSetUnhandledExceptionFilter();
}

#endif //defined(WIN32)

