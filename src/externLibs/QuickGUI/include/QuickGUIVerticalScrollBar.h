#ifndef QUICKGUIVERTICALSCROLLBAR_H
#define QUICKGUIVERTICALSCROLLBAR_H

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIButton.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport VerticalScrollBar :
		public Widget
	{
	public:
		friend class ScrollPane;

		enum ButtonLayout
		{
			BUTTON_LAYOUT_ADJACENT_UP		=  0,
			BUTTON_LAYOUT_ADJACENT_DOWN			,
			BUTTON_LAYOUT_MULTIPLE_BOTH			,
			BUTTON_LAYOUT_MULTIPLE_UP			,
			BUTTON_LAYOUT_MULTIPLE_DOWN			,
			BUTTON_LAYOUT_NONE					,
			BUTTON_LAYOUT_OPPOSITE			
		};
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
			@note
				Vertical or Horizontal TrackBars are derived from a comparison between width and height.
        */
		VerticalScrollBar(const std::string& name, GUIManager* gm);

		// Same as setValue, except that the scroll event is not fired.
		void _setValue(float value);

		/**
		* Add user defined event that will be called when amount of progress has changed.
		*/
		template<typename T> void addOnScrollEventHandler(void (T::*function)(const EventArgs&), T* obj)
		{
			mOnScrollHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnScrollEventHandler(MemberFunctionSlot* function);

		ButtonLayout getButtonLayout();
		float getLargeChange();
		/**
		* Gets the amount of time the left mouse button is down over a button or bar
		* before another scroll occurs.
		*/
		float getRepeatTime();
		Size getScrollButtonSize();
		float getSliderHeight();
		float getSmallChange();
		/**
		* Gets the numerical value representing the position of the left end of the slider, relative to the track bounds.
		* NOTE: value should be between 0.0 and 1.0
		*/
		float getValue();

		void scrollUpLarge();
		void scrollUpSmall();
		void scrollDownLarge();
		void scrollDownSmall();
		void setButtonLayout(ButtonLayout layout);
		void setLargeChange(float change);
		/**
		* Sets the amount of time the left mouse button is down over a button or bar
		* before another scroll occurs.
		*/
		void setScrollRepeatTime(float timeInSeconds);
		void setSmallChange(float change);
		/**
		* Sets the numerical value representing the position of the left end of the slider, relative to the track bounds.
		* NOTE: value should be between 0.0 and 1.0
		*/
		void setValue(float value);

		void timeElapsed(const float time);

	protected:
		~VerticalScrollBar();

		// time in seconds, ie 0.5
		float mScrollRepeatTime;
		float mRepeatTimer;

		// last recorded slider position
		Point mSliderPosition;

		float mMinSliderPosition;
		float mMaxSliderPosition;
		void _determineMinMax();

		float mLargeChange;
		float mSmallChange;

		ButtonLayout mButtonLayout;

		Button* mSlider;
		std::string mSliderTextureName;

		void setSliderHeight(float relativeHeight);
		void _constrainSlider();

		std::string mScrollUpTextureName;
		std::string mScrollDownTextureName;

		Button* mScrollUp1;
		Button* mScrollUp2;
		Button* mScrollDown1;
		Button* mScrollDown2;

		void _scroll(float change, ScrollEventArgs args);
		void onScrollUpDown(const EventArgs& args);
		void onScrollDownDown(const EventArgs& args);

		bool mMouseDownOnTrack;
		MouseButtonID mButtonDown;

		void onMouseDownOnTrack(const EventArgs& args);
		void onMouseUpOnTrack(const EventArgs& args);
		void onMouseLeaveTrack(const EventArgs& args);
		void onSizeChanged(const EventArgs& args);
		void onSliderDragged(const EventArgs& args);

		EventHandlerArray mOnScrollHandlers;

		void _showButtons();
	};
}

#endif
