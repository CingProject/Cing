#ifndef QUICKGUIBORDER_H
#define QUICKGUIBORDER_H

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
	class _QuickGUIExport Border :
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
		Border(const std::string& name, GUIManager* gm);

		/*
		* Allows widgets to notify their borders their skin component has changed,
		* which is used for situations such as Button down/over/default states, which
		* affect a border's appearance. (skim component)
		*/
		void _notifyParentSkinComponent(const std::string& skinComponent);

		BorderType getBorderType();
		int getOverlap();
		int getThickness();

		void setBorderType(BorderType t);
		void setOverlap(int lengthInPixels);
		void setThickness(int lengthInPixels);

	protected:
		virtual ~Border();
		virtual void setParent(Widget* parent);
	protected:

		BorderType mBorderType;

		float mThickness;
		float mOverlap;

		// used to restore mouse cursor when leaving.
		std::string mMouseCursorTexture;

		void onDragged(const EventArgs& args);
		void onMouseEnter(const EventArgs& args);
		void onMouseLeave(const EventArgs& args);
	};
}

#endif
