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
#ifndef __betacairo_cairomanager_H__
#define __betacairo_cairomanager_H__

#include "BetaCairo_Include.h"
#include "BetaCairo_Drawing.h"

namespace BetaCairo {

	class _BetaCairoExport CairoManager : public Ogre::Singleton<CairoManager> {

	public:

		CairoManager();
		~CairoManager();

		Drawing*	createDrawing(std::string name, unsigned int width, unsigned int height, PixelFormat format, unsigned char mipmaps = 2);
		void		destroyDrawing(std::string name);

		/// Set Dots per Inch
		///	@micro
		/// @param dpiValue Dots per Inch
		void	setDPI(float dpiValue = 96.0f) {
			dpiValue <= 0 ? setting_DPI = 96.0f : setting_DPI = dpiValue;
			setting_PT = dpiValue / 72.0f;
			setting_PICA = setting_PICA * 12.0f;
			setting_CM = dpiValue / 2.54f;
			setting_MM = setting_CM / 10.0f;

			std::cout << 
				"DPI  => " << setting_DPI << std::endl <<
				"PT   => " << setting_PT << std::endl <<
				"PICA => " << setting_PICA << std::endl <<
				"CM   => " << setting_CM << std::endl <<
				"MM   => " << setting_MM << std::endl;
		}

		/// Get Dots per inch
		/// @micro
		/// @return Dots per inch
		float	getDPI() {
			return setting_DPI;}

		Ogre::MaterialPtr	attachDrawingToANewMaterial(std::string materialName, Drawing *drawing, std::string resourceGroup = "General") {
				Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(materialName, resourceGroup);
				material->getTechnique(0)->getPass(0)->createTextureUnitState(drawing->getName());
				if (drawing->getPixelFormat() == BetaCairo::PF_RGBA)
					material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
				return material;
		}
	
		/// Get Pixel Length of
		/// @micro
		/// @return Returns the number of pixels of x unit.
		float	getPixelLength(float distance, Unit unit = PIXEL) {
			switch(unit) {
				case PIXEL:	return distance;
				case INCH:	return distance * setting_DPI;
				case PT:	return distance * setting_PT;
				case PICA:	return distance * setting_PICA;
				case CM:	return distance * setting_CM;
				case MM:	return distance * setting_MM;
			}
			return distance;
		}
	
	protected:

			void	_destroyAll();


	/// Variables

	public:
			

	private:

			/// Pixels per inch
			float	setting_DPI;
			
			/// Pixels per Point
			float	setting_PT;
			
			/// Pixels per Pica
			float	setting_PICA;
			
			/// Pixels per millimetre
			float	setting_MM;

			/// Pixels per centimetre
			float	setting_CM;

			/// Drawings map
			std::map<std::string, BetaCairo::Drawing*> mDrawings;
			
			/// Local pointer to Ogre::Root
			Ogre::Root *mRoot;
	};

};

#endif