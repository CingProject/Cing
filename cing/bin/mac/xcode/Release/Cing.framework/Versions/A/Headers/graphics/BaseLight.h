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
		bool				isValid     	() const { return m_bIsValid; }
		const Vector&		getPosition 	() const;
		Ogre::Light*		getOgreLight	()	{ return m_pLight; }
		const Color&		getDiffuseColor	() const { return m_diffuseColor; }
		const Color&		getSpecularColor() const { return m_specularColor; }



	private:

		// Constant / static attributes
		static const std::string	DEFAUTL_LIGHT_NAME; ///< Default name of the lights
		static long					m_lightCounter;     ///< Used to generate unique names for the lights

		// Attributes
		std::string					m_lightName;  		///< Name of this light
		Ogre::Light*				m_pLight;     		///< Pointer to the light object
		Color						m_diffuseColor;		///< Diffuse color of the light
		Color						m_specularColor;	///< Specular color of the light

		// Debugging purposes
		Ogre::BillboardSet*			m_lightFlareSet;	///< Used to draw a billboard where the light is (to debug its position)
		Ogre::Billboard*			m_lightFlare;		///< Used to draw a billboard where the light is (to debug its position)

		bool						m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};
} // namespace Cing

#endif // _BaseLight_H_
