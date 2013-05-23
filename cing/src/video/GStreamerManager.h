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

#ifndef _GStreamerManager_H_
#define _GStreamerManager_H_

// Precompiled headers
#include "Cing-Precompiled.h"

// GS
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

// Graphics
#include "graphics/Image.h"

namespace Cing
{
	/**
	 * Manages Common GStreamer stuff
	 */
	class GStreamerManager
	{
	public:
		static bool initGStreamer();

	private:

		// Internal stuff
		static String	m_pluginsPath;		///< Relative (to app) path to GStreamer plugins 
		static bool		m_GSInitialized;	///< Indicates whether GStreamer has been initialized or not

	};

} // namespace

#endif // _GStreamerManager_H_
