#ifndef QUICKGUIIMAGE_H
#define QUICKGUIIMAGE_H

#include "OgreResource.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	/** Represents a simple Imagel.
		@remarks
		Pretty much a Label, but without text.
		@note
		Images also support Render To Texture.
	*/
	class _QuickGUIExport Image :
		public Widget
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				material Ogre material defining the widget image.
			@param
				group QuadContainer containing this widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		Image(const std::string& name, GUIManager* gm);

		std::string getMaterialName();

		virtual bool overTransparentPixel(const Point& mousePixelPosition) const;

		/**
		* Applies the texture to the Quad if exists in some form, and updates the Image used for
		* transparency picking.
		*/
		virtual void setMaterial(const std::string& materialName);

		virtual void setSkin(const std::string& skinName, bool recursive = false);

        void setTexture(const std::string& textureName);

	protected:
		virtual ~Image();

		std::string mMaterialName;
		Ogre::ResourcePtr mWrapper;
	};
}

#endif
