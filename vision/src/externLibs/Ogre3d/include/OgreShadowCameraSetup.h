/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2006 Torus Knot Software Ltd
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
#ifndef __ShadowCameraSetup_H__
#define __ShadowCameraSetup_H__

#include "OgrePrerequisites.h"
#include "OgreMovablePlane.h"
#include "OgreSharedPtr.h"


namespace Ogre {

    /** This class allows you to plug in new ways to define the camera setup when
		rendering and projecting shadow textures.
	@remarks
		The default projection used when rendering shadow textures is a uniform
		frustum. This is pretty straight forward but doesn't make the best use of 
		the space in the shadow map since texels closer to the camera will be larger, 
		resulting in 'jaggies'. There are several ways to distribute the texels
		in the shadow texture differently, and this class allows you to override
		that. 
	@par
		Ogre is provided with several alternative shadow camera setups, including
		LiSPSM (LiSPSMShadowCameraSetup) and Plane Optimal (PlaneOptimalShadowCameraSetup).
		Others can of course be written to incorporate other algorithms. All you 
		have to do is instantiate one of these classes and enable it using 
		SceneManager::setShadowCameraSetup (global) or Light::setCustomShadowCameraSetup
		(per light). In both cases the instance is wrapped in a SharedPtr which means
		it will  be deleted automatically when no more references to it exist.
	@note
        Shadow map matrices, being projective matrices, have 15 degrees of freedom.
		3 of these degrees of freedom are fixed by the light's position.  4 are used to
		affinely affect z values.  6 affinely affect u,v sampling.  2 are projective
		degrees of freedom.  This class is meant to allow custom methods for 
		handling optimization.
    */
	class _OgreExport ShadowCameraSetup
	{
	public:
		/// Function to implement -- must set the shadow camera properties
		virtual void getShadowCamera (const SceneManager *sm, const Camera *cam, 
									  const Viewport *vp, const Light *light, Camera *texCam) const = 0;
		/// Need virtual destructor in case subclasses use it
		virtual ~ShadowCameraSetup() {}

	};



	/** Implements default shadow camera setup
        @remarks
            This implements the default shadow camera setup algorithm.  This is what might
			be referred to as "normal" shadow mapping.  
    */
	class _OgreExport DefaultShadowCameraSetup : public ShadowCameraSetup
	{
	public:
		/// Default constructor
		DefaultShadowCameraSetup();
		/// Destructor
		virtual ~DefaultShadowCameraSetup();

		/// Default shadow camera setup
		virtual void getShadowCamera (const SceneManager *sm, const Camera *cam, 
									  const Viewport *vp, const Light *light, Camera *texCam) const;
	};



	typedef SharedPtr<ShadowCameraSetup> ShadowCameraSetupPtr;

}

#endif
