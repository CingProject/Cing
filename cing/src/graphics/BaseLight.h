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

#ifndef _BaseLight_H_
#define _BaseLight_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "SceneGraphElement.h"

#include "GraphicsPrereqs.h"
#include "graphics/Color.h"

// Common
#include "common/CommonTypes.h"

namespace Cing
{

	/**
	* @internal
	* Base class for all the lights
	*/
	class BaseLight: public SceneGraphElement
	{
	public:

		// Constructor / Destructor
		BaseLight();
		virtual ~BaseLight();

		// Init / Release / Update
		bool  	init              	( float r, float g, float b, float x, float y, float z );
		void  	end               	();

		// Global control
		void	enable				( bool enable );
		bool	isEnabled			() const;

		// Set methods
		void  	setPosition       	( float x, float y, float z );
		void  	setPosition       	( float x, float y );
		void  	setPosition       	( const Vector& pos );

		void  	setDirection      	( float x, float y, float z );
		void  	setDirection      	( float x, float y );
		void  	setDirection      	( const Vector& dir );

		void  	setDiffuseColor   	( float r, float g, float b );
		void  	setDiffuseColor   	( const Color& color );

		void  	setSpecularColor  	( float r, float g, float b );
		void  	setSpecularColor  	( const Color& color );

		void  	setLightType      	( Light::LightTypes type );

		void  	setAttenuation		( float range, float constant, float linear, float quadratic );
		void	setAttenuation		( float range );

		// Shadows
		void	castShadows			( bool cast );

		// Debug methods
		void	drawDebug			( bool draw = true);

		// Query methods
		bool				isValid     () const { return m_bIsValid; }
		const Vector&		getPosition () const;
		Ogre::Light*		getOgreLight()	{ return m_pLight; }



	private:

		// Constant / static attributes
		static const std::string	DEFAUTL_LIGHT_NAME; ///< Default name of the lights
		static long					m_lightCounter;     ///< Used to generate unique names for the lights

		// Attributes
		std::string					m_lightName;  		///< Name of this light
		Ogre::Light*				m_pLight;     		///< Pointer to the light object

		// Debugging purposes
		Ogre::BillboardSet*			m_lightFlareSet;	///< Used to draw a billboard where the light is (to debug its position)
		Ogre::Billboard*			m_lightFlare;		///< Used to draw a billboard where the light is (to debug its position)

		bool						m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};
} // namespace Cing

#endif // _BaseLight_H_
