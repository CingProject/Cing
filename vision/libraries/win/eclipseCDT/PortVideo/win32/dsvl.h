/* ========================================================================
*  PROJECT: DirectShow Video Processing Library (DSVL)
*  Version: 0.0.8 (05/13/2005)
*  ========================================================================
*  Author:  Thomas Pintaric, Vienna University of Technology
*  Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
*  =======================================================================
* 
*  Copyright (C) 2005  Vienna University of Technology
* 
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*  For further information please contact Thomas Pintaric under
*  <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
*  Vienna University of Technology, Favoritenstr. 9-11/E188/2, A-1040
*  Vienna, Austria.
* ========================================================================*/

#define DSVL_VERSION 0x00000802 // 0.0.8b

#ifdef DSVL_EXPORTS
#define DSVL_API __declspec(dllexport)
#else
#define DSVL_API __declspec(dllimport)
#endif

#include <Windows.h>
#include "DSVL_PixelFormatTypes.h"

// ------------------------------------------------------------
#ifndef __MEMORY_BUFFER_HANDLE__
#define __MEMORY_BUFFER_HANDLE__

#define MIN_ALLOCATOR_BUFFERS_PER_CLIENT  3
#define DEF_CONCURRENT_CLIENTS            3
struct MemoryBufferHandle
{
	unsigned long  n; // sample number
	LONGLONG t; // timestamp
};

#endif

#define _INVALID_TIMESTAMP 0;
// ------------------------------------------------------------

class DSVL_API DSVL_VideoSource
{
public:
	DSVL_VideoSource();
	~DSVL_VideoSource();

	HRESULT BuildGraphFromXMLString(char* xml_string = NULL);
	HRESULT BuildGraphFromXMLFile(char* xml_filename = NULL);
	HRESULT ReleaseGraph();

	HRESULT EnableMemoryBuffer(unsigned int _maxConcurrentClients = DEF_CONCURRENT_CLIENTS,
                               unsigned int _allocatorBuffersPerClient = MIN_ALLOCATOR_BUFFERS_PER_CLIENT);
	HRESULT DisableMemoryBuffer();

	bool IsGraphInitialized();
	DWORD WaitForNextSample(long dwMilliseconds = INFINITE); 

	HRESULT CheckoutMemoryBuffer(MemoryBufferHandle* pHandle, 
		BYTE** Buffer,
		unsigned int *Width = NULL,
		unsigned int *Height = NULL,
		PIXELFORMAT* PixelFormat = NULL,
		LONGLONG* Timestamp = NULL);

	HRESULT CheckinMemoryBuffer(MemoryBufferHandle Handle, bool ForceRelease = false); 

	HRESULT GetCurrentMediaFormat(LONG* frame_width,
                                  LONG *frame_height,
								  double* frames_per_second,
								  PIXELFORMAT* pixel_format);
	LONGLONG GetCurrentTimestamp();

	// media flow control
	HRESULT Run();
	HRESULT Pause();
	HRESULT Stop(bool forcedStop = false);

	HRESULT ShowFilterProperties();

private:
	void* p_graphManager;
};