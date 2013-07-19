/*

	BetaCairo a wrapper for the Cairo Graphics library
	Copyright (C) 2005, 2006 Robin Southern http://get.nxogre.org/cetacairo/

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef __betacairo_drawing_H__
#define __betacairo_drawing_H__

#include "BetaCairo_Include.h"
#include "BetaCairo_Drawing.h"
#include "BetaCairo_CairoSurface.h"


namespace BetaCairo {

	class _BetaCairoExport Drawing : public CairoSurface {

		public:

			Drawing(std::string name, unsigned int width, unsigned int height, PixelFormat format, bool nSquared = false, unsigned char mipmaps = 2);
			~Drawing();


			/// Return's the name of the surface
			/// @micro
			/// @return The name
			std::string	getName() {
				return mName;}

			void update();

		protected:
			
			std::string							mName;

			Ogre::TexturePtr					mTexturePtr;
			Ogre::HardwarePixelBufferSharedPtr	mPixelBuffer;

			unsigned int						mActualWidth;
			unsigned int						mActualHeight;
			unsigned char						mMipMaps;

		private:
	
	};

};

#endif