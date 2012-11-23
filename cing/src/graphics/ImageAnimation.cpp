/*
This source file is part of the Cing project
For the latest info, see http://www.cing.cc

Copyright (c) 2006-2011 Julio Obelleiro and Jorge Cano

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


// Precompiled headers
#include "Cing-Precompiled.h"

#include "ImageAnimation.h"

// Common
#include "common/CommonUtilsIncludes.h"


namespace Cing
{

	/**
	 * @brief Loads an image sequence/animation to be played
	 *
	 * @note The image files specified should be placed on the data directory of the application and numbered sequencially
	 *
	 * @note Supported image formats are: .bmp, .jpg, .gif, .raw, .png, .tga and .dds.
	 *
	 * @param baseName		Base name for the image files (they must be inside the data folder, but could be in subdirectories).
	 *						For example, a baseName "fx/myImage.png" with number of frames 12, would mean that in there is 
	 *						a directory called fx inside the data folder, and the images would range from myImage_0.png to myImage_11.png
	 * @param nFrames		Number of frames in the animation
	 * @param durationSecs	Duration in seconds for the whole secuence to be played.
	 */
	bool ImageAnimation::load( const std::string& baseName, int nFrames, float durationSecs )
	{
		m_nFrames = nFrames;
		// File the last dot to extract the base image name (with no extension)
		size_t lastDot = baseName.find_last_of( "." );
		if ( lastDot == std::string::npos )
		{
			LOG_ERROR( "ImageAnimation::load Could not load the animation. baseName does not contain any dot/extension" );
			return false;
		}

		// Build the name for the first image (to load it)
		std::string baseNameNotExtension	= baseName.substr(0, lastDot);
		std::string extension				= baseName.substr(lastDot);

		std::string firstFrame = baseNameNotExtension + "_0" + extension;

		// Load the first image to make sure the texture has the right dimensions/format
		Image::load(firstFrame);
		if ( isValid() )
		{
			// Set the animation material
			Ogre::MaterialPtr material = getTexturedQuad().getMaterial();
			if ( !material.isNull() )
			{
				material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAnimatedTextureName( baseName, nFrames, durationSecs );
				//material->getTechnique(0)->getPass(0)->setSceneBlendingOperation( Ogre::SBO_ADD );
			}
			else
			{
				LOG_ERROR( "ImageAnimation::load Could not load the animation. Null material found." );
				end();
				return false ;
			}
		}

		return true;
	}


	/**
	 * Plays the animation
	 */
	void ImageAnimation::play()
	{
		Ogre::MaterialPtr material = getTexturedQuad().getMaterial();
		if ( !material.isNull() )
		{
			Ogre::Controller<Ogre::Real>* controller = material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->_getAnimController();
			if ( controller )
			{
				controller->setEnabled(true);
			}
		}
		else
		{
			LOG_ERROR( "ImageAnimation::jump: invalid animation. Did you call load() to initialize it?" );
			return;
		}
	}

	/**
	 * Stops the animation (returns to frame 0)
	 */
	void ImageAnimation::stop()
	{
		Ogre::MaterialPtr material = getTexturedQuad().getMaterial();
		if ( !material.isNull() )
		{
			Ogre::Controller<Ogre::Real>* controller = material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->_getAnimController();
			if ( controller )
			{
				controller->setEnabled(false);
			}
		}
		else
		{
			LOG_ERROR( "ImageAnimation::jump: invalid animation. Did you call load() to initialize it?" );
			return;
		}
	}


	/**
	 * Returns the duration of the animation in seconds
	 * @return duration of the animation in seconds
	 */
	float ImageAnimation::getAnimationDuration()
	{
		Ogre::MaterialPtr material = getTexturedQuad().getMaterial();
		if ( !material.isNull() )
			return material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getAnimationDuration();
		else
		{
			LOG_ERROR( "ImageAnimation::getAnimationDuration: Requesting animation duration on an invalid animation. Did you call load() to initialize it?" );
			return 0.0f;
		}
	}
	/**
	 * @brief Set the current frame of the animation. Useful when duration = 0 so the animation is controlled manually
	 * @param newCurrentFrame		new current frame of the animation
	 */
	bool ImageAnimation::setCurrentFrame(int newCurrentFrame)
	{
		Ogre::MaterialPtr material = getTexturedQuad().getMaterial();
		if ( !material.isNull()){
			if( newCurrentFrame >=0 ){
				material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setCurrentFrame(newCurrentFrame);
				return true;
			}else{
				LOG_ERROR( "ImageAnimation::setCurrentFrame: newCurrentFrame < 0" );
				return false;
			}
		}else
		{
			LOG_ERROR( "ImageAnimation::getAnimationDuration: Requesting animation duration on an invalid animation. Did you call load() to initialize it?" );
			return false;
		}
	}
	

} // namespace Cing
