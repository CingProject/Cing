/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#pragma once
	
/// This file simplified the use of the OpenCV module, adding includes and the pragmas to link with the required openCV libraries

// Includes
#include "OpenCV/src/BackgroundSubtraction.h"
#include "OpenCV/src/BlobTracker.h"
#include "OpenCV/src/MediaPlayerOCV.h"
#include "OpenCV/src/OpenCVUtils.h"
#include "OpenCV/src/Renderer2DOpenCV.h"

// Linker 
#if defined(WIN32)
#ifdef _DEBUG
		// Ligs
		#pragma comment(lib,"OpenCV/lib/lib/vs2012/opencv_core246d.lib")
		#pragma comment(lib,"OpenCV/lib/lib/vs2012/opencv_highgui246d.lib")
		#pragma comment(lib,"OpenCV/lib/lib/vs2012/opencv_imgproc246d.lib")
#else
		#pragma comment(lib,"opencv_core246.lib")
		#pragma comment(lib,"opencv_highgui246.lib")
		#pragma comment(lib,"opencv_imgproc246.lib")
#endif
#endif 

// DLLs for runtime

namespace Cing
{
	// Global OpenCV variables
	Renderer2DOpenCV	renderer2DOpenCV;


	// Init / Release of anything the Opencv plugin needs
	void initOpenCV	()
	{
#if defined(WIN32)
		// This could be used if DLL loading is delayed
		//SetDllDirectory( L"C:/Users/Julio/Desktop/Proyectos - Code/Cing/git/cing-ings/OpenCV/lib/bin/vs2012" );
#endif
	}

	// Sets Renderer2DOpenCV as the default Cing 2D renderer (will be used from now on)
	void enableOpenCVRenderer2D()
	{
		renderer2DOpenCV.init();
		setRenderer2D( &renderer2DOpenCV );
	}
}

