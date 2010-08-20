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

#ifndef _VICamera_h_
#define _VICamera_h_

#include "CameraInputPrereqs.h"
#include "BaseCameraInput.h"
#include "VideoInput/include/videoInput.h"


namespace Cing
{

/**
 * @internal
 * @brief Class to capture camera input based on Video Input library
 */
class VICamera: public BaseCameraInput
{
public:

	// Constructor / Destructor
	VICamera();
	~VICamera();

	// Init / Release / Update
	void	init 	( int deviceId, int width, int height, int fps, GraphicsType format, bool multithreaded = true );
	void	update();
	void	end		();	

	// Query  Methods
	bool					isValid			 () { return m_bIsValid; }
	void					listDevices	 ();
	videoInput&				getVideoInput() { return m_viCamera; }

private:

	// Attributes
	videoInput			m_viCamera;		///< VideoInput object to communicate with the camera
	bool						m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _VICamera_h_