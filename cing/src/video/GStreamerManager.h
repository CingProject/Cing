/*
This source file is part of the Cing project
For the latest info, see http://www.cing.cc

Copyright (c) 2008-2010 Julio Obelleiro and Jorge Cano

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
