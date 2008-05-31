#ifndef QUICKGUIQUAD_H
#define QUICKGUIQUAD_H

#include "QuickGUIForwardDeclarations.h"

#include "QuickGUIExportDLL.h"
#include "QuickGUIRect.h"
#include "QuickGUITypedef.h"
#include "QuickGUIVector4.h"
#include "QuickGUIVertex.h"

#include <string>

namespace QuickGUI
{
	class _QuickGUIExport Quad
	{
	public:
		typedef enum ClipMode
		{
			CLIPMODE_GREATGRANDPARENT	=  0, // Used for Borders
			CLIPMODE_NONE					, // Used for Lists and Drag/Drop Items
			CLIPMODE_PARENT					  // Used for Text and Texture Quads
		};

		/**
		* Used to determine Render Ordering in Quad Container.  Menu Layer is rendered last.
		*/
		typedef enum Layer
		{
			LAYER_CHILD		=  0,
			LAYER_MENU
		};
	public:
		Quad(Widget* owner);
		Quad(GUIManager* gm);
		~Quad();

		void _notifyAddedToRenderObjectGroup();
		// Allows the renderer to notify the quad its changes were accepted.
		void _notifyChangesHandled();
		void _notifyRemovedFromQuadContainer();
		void _notifyQuadContainer(QuadContainer* container);
		void _notifyQuadContainerNeedsUpdate();
		// Checks if the Vertex data needs to be updated.
		void _update();

		void addToRenderObjectGroup();

		bool dimensionsChanged();

		/*
		* Returns true if this quad clips to its QuadContainer's Widget's dimensionsl,
		* false otherwise.
		*/
		ClipMode getClipMode();
		Rect getClippedDimensions();
		Rect getClippingRect();
		Rect getDimensions();
		bool getInheritLayer();
		Layer getLayer();
		std::string getMaterialName();
		float getOpacity();
		bool getShowWithOwner();
		Vertex* getVertices();
		const int getOffset() const;
		Point getPosition();
		Size getSize();

		bool isPointWithinBounds(const Point& pixelPosition) const;

		void removeFromRenderObjectGroup();

		void setClipMode(ClipMode m);
		void setColor(const Ogre::ColourValue& color);
		void setColor(const Ogre::ColourValue& topColor, const Ogre::ColourValue& botColor);
		void setDimensions(const Rect& pixelDimensions);
		void setGUIManager(GUIManager* gm);
		void setHeight(float pixelHeight);
		void setInheritLayer(bool inherit);
		void setLayer(Layer l);
		void setMaterial(const std::string& materialName);
		void setOffset(int offset);
		void setOpacity(float opacity);
		void setPosition(const Point& pixelPosition);
		void setShowWithOwner(bool showWithOwner);
		void setSize(const Size& pixelSize);
		void setTextureCoordinates(Vector4 textureCoordinates);
		void setVisible(bool visible);
		void setWidth(float pixelWidth);
		void setXPosition(float pixelX);
		void setYPosition(float pixelY);

		bool materialChanged();

		inline bool visible() const
		{
			return mVisible;
		}

		bool offsetChanged();

	protected:
		Widget*				mOwner;
		GUIManager*			mGUIManager;
		QuadContainer*		mQuadContainer;
		Ogre::RenderSystem* mRenderSystem;

		Layer				mLayer;
		bool				mInheritQuadLayer;

		std::string			mMaterialName;
		// number of parent iterations away from sheet quad.
		int					mOffset;
		Rect				mPixelDimensions;
		// UV coordinates: (left,top,right,bottom)
		Vector4				mTextureCoordinates;
		bool				mVisible;
		bool				mShowWithOwner;
		Ogre::ColourValue	mTopColor;
		Ogre::ColourValue	mBottomColor;

		ClipMode			mClipMode;
		// Store changes made via Clipping.
		Rect				mDimensionsViaClipping;
		Vector4				mTextureCoordinatesViaClipping;

		bool				mAddedToRenderGroup;
		bool				mColorChanged;
		bool				mDimensionsChanged;
		bool				mMaterialChanged;
		bool				mTextureCoordsChanged;
		bool				mOffsetChanged;

		Vertex				mVertices[VERTICES_PER_QUAD];

		/*
		* Resizes and adjusts texture coords to display any visible portions that lie
		* inside Rect r.
		* NOTE: If Quad lies outside Rect r, it will not be clipped, but hidden.
		* NOTE: If Quad lies inside Rect r, no clipping will occur.
		*/
		void _clip();
		void _computeVertices();
		void _updateTextureCoords();
		void _updateVertexColor();
	};
}

#endif
