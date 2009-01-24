#ifndef QUICKGUIIMAGESET_H
#define QUICKGUIIMAGESET_H

#include "OgreCodec.h"
#include "OgreException.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreMaterialManager.h"
#include "OgrePass.h"
#include "OgrePrerequisites.h"
#include "OgreRenderSystem.h"
#include "OgreResourceGroupManager.h"
#include "OgreRoot.h"
#include "OgreTechnique.h"
#include "OgreTextureManager.h"
#include "OgreLogManager.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIPoint.h"
#include "QuickGUISize.h"
#include "QuickGUIVector4.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

namespace QuickGUI
{
	/**
	* Stores a generated SkinSet from the individual widget images
	*/
	class _QuickGUIExport SkinSet
	{
	public:
		friend class SkinSetManager;

		enum ImageType
		{
			IMAGE_TYPE_PNG		=  0,
			IMAGE_TYPE_JPG			,
			IMAGE_TYPE_BMP
		};
	public:
		/*
		* Adds a texture name to the list of textures that will be included in the
		* SkinSet texture.
		* NOTE: Texture will not be present until buildTexture function is executed.
		*/
		void addTexture(const std::string& textureName, const Vector4 &texCoord = Vector4::ZERO);

		/*
		* Removes a texture name to the list of textures that will be included in the
		* SkinSet texture.
		* NOTE: Texture will not be removed until buildTexture function is executed.
		*/
		void removeTexture(const std::string& textureName);

		/*
		* Builds the resulting Image from all added Images.  If Texture has already
		* been built, it will be rebuilt.  If you have added images to the SkinSet,
		* they will not be in the texture until you call this function.
		*/
		void buildTexture();

		/*
		* Compute texture coordinates inside the Skinset for each texture.
		*/
		void buildTextureCoordinates(std::vector<Ogre::Image> &images);

		/*
		* Returns true if SkinSet Texture contains texture, false otherwise.
		*/
		bool containsImage(std::string textureName);

		Ogre::Image* getImage(const std::string& textureName);
		std::string getImageExtension() const;
		/*
		* Returns the pixel height of the embedded image or texture.
		*/
		int getImageHeight(const std::string& textureName);
		Point getImagePosition(const std::string& textureName);
		Size getImageSize(const std::string& textureName);
		/*
		* Returns the pixel width of the embedded image or texture.
		*/
		int getImageWidth(const std::string& textureName);
		/*
		* Retrieve material used for this Skinset.
		*/
		std::string getMaterialName() { return mMaterialName; }
		std::string getSkinName() const;
		// Return the name of the skin for this SkinSet
		std::string getTextureName() const;

		/*
		* Return the UV coordinates of the image, assuming the image is a part of the image set.
		* Return form is (left,top,right,bot). (left and right refer to x-coordinate, top and
		* bottom refer to y-coordinates)
		*/
		Vector4 getTextureCoordinates(const std::string &imageName) const;
		/*
		* set the UV coordinates of the image, assuming the image is a part of the image set.
		* Vector4 form is (left,top,right,bot). (left and right refer to x-coordinate, top and
		* bottom refer to y-coordinates)
		*/
		void setTextureCoordinates(const std::string &imageName, const Vector4 &texCoord);
		/*
		* saves skin to a .skinset file in the folder containing first
		* place found in the resource group.
		*/
		void saveSkin();
		/*
		* loads a skin from a .skinset file return false if failing
		*/
		bool loadSkin();

		/** Returns true if the relative coordinates are over a transparent pixel, false otherwise.
            @param
                skinComponent The name to be embedded Image to query.
            @param
                xPos The relative position of the horizontal pixel in the image.
			@param
				yPos The relative position of the vertical pixel in the image.
        */
		bool overTransparentPixel(const std::string& skinComponent, int xPos, int yPos) const;

		/*
		* set material used for this Skinset.
		*/
		void setMaterialName(const std::string& materialName){ mMaterialName = materialName; }

		const Ogre::StringVector &getTextureNames() const {return mTextureNames;}


	protected:
		std::string mSkinName;

		// Corresponding to the Image Type. (IMAGE_TYPE_PNG -> ".png")
		std::string mImageExtension;

		// list of textures used to build the SkinSet Texture.
		Ogre::StringVector mTextureNames;
		// list of textures that are currently in the SkinSet Texture.
		std::set<std::string> mContainedTextures;

		// The actual texture that stores the imageset
		Ogre::uint mTextureWidth;
		Ogre::uint mTextureHeight;

		float mHorizontalTexelOffset;
		float mVerticalTexelOffset;

		// This map connects individual textures to its UV coordinates, within
		// the SkinSet Texture.
		std::map<std::string,Vector4> mTextureMap;

		// Number of images that this SkinSet has
		size_t mNumIndividualTextures;

		std::map<std::string,Ogre::Image*> mCreatedImages;

	private:
		// Generate a new SkinSet using the skin's image files.
		SkinSet(const std::string& skinName, ImageType t, const std::string &resourceGroup);
		// Delete this SkinSet
		~SkinSet();

		void _determineExtension(ImageType t);
		void buildMaterial();
		void _findSkinTextures();
		std::string mTextureName;
		std::string mResourceGroup;
		std::string mMaterialName;
		bool mDirtyTexture;
		bool mDirtyTextureCoordinates;

		Ogre::Image* mSkinSetImage;
	};
}

#endif
