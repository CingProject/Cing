#ifndef QUICKGUIWIDGET_H
#define QUICKGUIWIDGET_H

#include "OgreException.h"
#include "OgrePrerequisites.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIMemberFunctionPointer.h"
#include "QuickGUIRect.h"
#include "QuickGUIQuad.h"
#include "QuickGUIQuadContainer.h"
#include "QuickGUISkinSetManager.h"
#include "QuickGUIExportDLL.h"

#include <vector>
#include <ctype.h>

namespace QuickGUI
{
	// Forward declarations
	class Border;
	class Window;
	class Sheet;
	class Panel;
	class GUIManager;

	/** Main QuickGUI components for creating GUI.
		@remarks
		This class outlines the interface for
		widgets, providing default functionality.
	*/
	class _QuickGUIExport Widget
	{
	public:
		// GUIManager is the only manager that can destroy widgets.
		friend class GUIManager;
		friend class List;
		friend class Panel;
		friend class QuadContainer;
		friend class RadioButtonGroup;
		friend class ScrollPane;

		/**
		* Outlining Types of widgets in the library.
		*/
		enum Type
		{
			TYPE_BORDER				=  0,
			TYPE_BUTTON					,
			TYPE_CHECKBOX				,
			TYPE_COMBOBOX				,
			TYPE_CONSOLE				,
			TYPE_IMAGE					,
			TYPE_LABEL					,
			TYPE_LIST					,
			TYPE_MENULABEL				,
			TYPE_LABELAREA				,
			TYPE_TEXTAREA				,
			TYPE_NSTATEBUTTON			,
			TYPE_PANEL					,
			TYPE_PROGRESSBAR			,
			TYPE_RADIOBUTTON			,
			TYPE_SCROLL_PANE			,
			TYPE_SCROLLBAR_HORIZONTAL	,
			TYPE_SCROLLBAR_VERTICAL		,
			TYPE_SHEET					,
			TYPE_TEXTBOX				,
			TYPE_TITLEBAR				,
			TYPE_TRACKBAR_HORIZONTAL	,
			TYPE_TRACKBAR_VERTICAL		,
			TYPE_TREE					,
			TYPE_WINDOW
		};
		/**
		* All widgets must support these events
		*/
		enum Event
		{
			EVENT_CHARACTER_KEY		=  0,
			EVENT_CHILD_ADDED			,
			EVENT_CHILD_CREATED			,
			EVENT_CHILD_DESTROYED		,
			EVENT_CHILD_REMOVED			,
			EVENT_DISABLED				,
			EVENT_DRAGGED				,
			EVENT_DROPPED				,
			EVENT_ENABLED				,
			EVENT_GAIN_FOCUS			,
			EVENT_HIDDEN				,
			EVENT_KEY_DOWN				,
			EVENT_KEY_UP				,
			EVENT_LOSE_FOCUS			,
			EVENT_MOUSE_BUTTON_DOWN		,
			EVENT_MOUSE_BUTTON_UP		,
			EVENT_MOUSE_CLICK			,
			EVENT_MOUSE_CLICK_DOUBLE	,
			EVENT_MOUSE_CLICK_TRIPLE	,
			EVENT_MOUSE_ENTER			,
			EVENT_MOUSE_LEAVE			,
			EVENT_MOUSE_MOVE			,
			EVENT_MOUSE_WHEEL			,
			EVENT_PARENT_CHANGED		,
			EVENT_POSITION_CHANGED		,
			EVENT_SHOWN					,
			EVENT_SIZE_CHANGED			,
			EVENT_END_OF_LIST
		};
		/**
		* Specifies horizontal position/size relative to parent resizing.
		*/
		enum HorizontalAnchor
		{
			ANCHOR_HORIZONTAL_LEFT				=  0,
			ANCHOR_HORIZONTAL_RIGHT					,
			ANCHOR_HORIZONTAL_LEFT_RIGHT			,
			ANCHOR_HORIZONTAL_NONE
		};
		/**
		* Specifies vertical position/size relative to parent resizing.
		*/
		enum VerticalAnchor
		{
			ANCHOR_VERTICAL_TOP				=  0,
			ANCHOR_VERTICAL_BOTTOM				,
			ANCHOR_VERTICAL_TOP_BOTTOM			,
			ANCHOR_VERTICAL_NONE
		};

		enum BorderType
		{
			BORDER_TYPE_TOP_LEFT		=  0,
			BORDER_TYPE_TOP_RIGHT			,
			BORDER_TYPE_BOTTOM_LEFT			,
			BORDER_TYPE_BOTTOM_RIGHT		,
			BORDER_TYPE_LEFT				,
			BORDER_TYPE_TOP					,
			BORDER_TYPE_RIGHT				,
			BORDER_TYPE_BOTTOM
		};
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width, and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				textureName The name of the texture used to visually represent the widget. (ie "qgui.window.png")
			@param
				group The QuadContainer containing the Quad used by this Widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		Widget(const std::string& name, GUIManager* gm);

		virtual void addChild(Widget* w);
		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: EVENT_GAIN_FOCUS, EVENT_CHARACTER_KEY, EVENT_MOUSE_BUTTON_DOWN, etc
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addEventHandler(QuickGUI::EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addEventHandler(Event EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mUserEventHandlers[EVENT].push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addEventHandler(Event EVENT, MemberFunctionSlot* function);

		template<typename T> void addEventListener(void (T::*function)(const EventArgs&), T* obj)
		{
			mEventListeners.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addEventListener(MemberFunctionSlot* function);

		void allowResizing(bool allow);
		/**
		* Alters the widgets offset to be higher than widget w.  Widget w must be in the
		* same QuadContainer and Layer.
		*/
		virtual void appearOverWidget(Widget* w);
		void constrainDragging(bool DragXOnly, bool DragYOnly);
		/**
		* Disable Widget, making it unresponsive to events.
		* NOTE: Sheets cannot be disabled.
		*/
		virtual void disable();
		/**
		* Moves draggingWidget.  By default, dragging widget is this widget, but this can be changed.
		* Allows dragging the titlebar or it's text to drag the window, for example.
		*/
		void drag(const float& pixelX, const float& pixelY);
		/**
		* Returns true if the widget is able to be dragged, false otherwise.
		*/
		bool draggingEnabled() const;
		/**
		* Enable Widget, allowing it to accept and handle events.
		* NOTE: Sheets cannot be enabled/disabled
		*/
		virtual void enable();
		/**
		* Returns true is widget is enabled, false otherwise.
		*/
		bool enabled() const;
		/**
		* Enable or Disable dragging.
		*/
		void enableDragging(bool enable);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireEvent(Event e, EventArgs& args);
		/**
		* Sets focus to the widget by firing an activation event.
		*/
		virtual void focus();
		Rect getActualDimensions() const;
		float getActualOpacity() const;
		/**
		* Returns the position of the widget as it would be drawn on the screen.
		* NOTE: This is a convenience method. Actual Position is the same as
		*  getScreenPosition() + getScrollOffset().
		*/
		Point getActualPosition() const;
		Size getActualSize() const;
		Border* getBorder(BorderType t);
		const Border* getBorder(BorderType t) const;
		WidgetArray* getChildWidgetList();
		const WidgetArray* getChildWidgetList() const;
		Widget* getChildWidget(const std::string& name);
		const Widget* getChildWidget(const std::string& name) const;
		Widget* getChildWidget(Type t, unsigned int index);
		const Widget* getChildWidget(Type t, unsigned int index) const;
		Rect getClippedDimensions();
		Rect getDimensions() const;
		GUIManager* getGUIManager();
		const GUIManager* getGUIManager() const;
		int getNumberOfHandlers(Event e) const;
		bool getInheritOpacity() const;
		float getOpacity() const;
		Point getPosition() const;
		Point getScrollOffset() const;
		Size getSize() const;

		std::string getFontName() const;
		/**
		* Returns true if the widget will gain focus when clicked, false otherwise.
		*/
		bool getGainFocusOnClick() const;
		bool getGrabbed() const;
		float getHeight() const;
		/**
		* Returns true if this widget is hidden when its parent is hidden.
		*/
		bool getHideWithParent() const;
		/**
		* Iterates through all child widgets and retrieves the highest offset.
		*/
		int getHighestOffset() const;
		HorizontalAnchor getHorizontalAnchor() const;
		bool getInheritQuadLayer() const;
		std::string getInstanceName() const;
		/**
		* Returns true if window is able to be repositions, false otherwise.
		*/
		bool getMovingEnabled() const;
		/**
		* Returns the number of parent iterations required to get to Sheet widget.
		*/
		int getOffset() const;
		/**
		* Get Panel this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Panel* getParentPanel();
		const Panel* getParentPanel() const;
		/**
		* Get Sheet this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Sheet* getParentSheet();
		const Sheet* getParentSheet() const;
		/**
		* Get Widget this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Widget* getParentWidget();
		const Widget* getParentWidget() const;
		/**
		* Get Window this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Window* getParentWindow();
		const Window* getParentWindow() const;
		bool getPropogateEventFiring(Event e) const;
		/*
		* Get Render Object that visually represents the widget.
		*/
		Quad* getQuad();
		const Quad* getQuad() const;
		/*
		* Get Render Object Group this widget's Quad belongs in.
		*/
		virtual QuadContainer* getQuadContainer();
		const QuadContainer* getQuadContainer() const;
		Quad::Layer getQuadLayer();
		const Quad::Layer getQuadLayer() const;
		/*
		* Get the screen pixel coordinates this widget is drawn at.
		* NOTE: This is not the same as getPosition, which returns a value relative to parent.
		* NOTE: This may not be the actual screen coordinates, since QuickGUI supports scrolling.
		*/
		Point getScreenPosition() const;
		bool getScrollPaneAccessible() const;
		std::string getSkinComponent() const;
		/**
		* Get whether or not this widget is shown when its parent is shown.
		*/
		bool getShowWithParent() const;
		std::string getSkin() const;
		/**
		* Iterates through visible Children widgets to find and return the widget that is *hit* by the point.
		* Returns NULL is nothing is *hit*.
		*/
		virtual Widget* getTargetWidget(const Point& pixelPosition);
		virtual const Widget* getTargetWidget(const Point& pixelPosition) const;
		bool getUseTransparencyPicking() const;
		/**
		* Returns the type of the widget, as enumerated above. ex. TYPE_BUTTON.
		*/
		Type getWidgetType() const;
		VerticalAnchor getVerticalAnchor() const;
		float getWidth() const;
		float getXPosition() const;
		float getYPosition() const;
		bool hasMouseButtonHandlers() const;
		/**
		* Sets mVisible to false.  Widgets should override this to implement how they handle
		* hiding.
		*/
		virtual void hide();
		void hideSkin();
		/**
		* Returns true if pixel point p is inside the pixel dimensions of this widget.
		*/
		virtual bool isPointWithinBounds(const Point& pixelPosition) const;
		bool isVisible() const;
		/**
		* Returns true if Widget w is a child of this widget, false otherwise.
		*/
		bool isChild(const Widget* w) const;

		/**
		* Offset the widget position.  Useful for dragging/moving widgets.
		*/
		void move(const float& pixelX, const float& pixelY);
		void move(const Point& pixelOffset);
		void moveX(float pixelX);
		void moveY(float pixelY);
		/*
		* Function disabling ability to change widget's texture.
		*/
		void lockTexture();
		/**
		* Determins if the mouse if over a transparent part of the image defining the widget.
		* Used to determine if the mouse is *over* a widget. (non transparent parts)
		*/
		virtual bool overTransparentPixel(const Point& mousePixelPosition) const;
		/**
		* Force updating of the Widget's Quad position on screen.
		*/
		virtual void redraw();
		void removeChild(Widget* w);
		void removeChild(const std::string& widgetName);
		/**
		* Properly cleans up all child widgets.
		*/
		virtual void removeAndDestroyAllChildWidgets();
		void removeAndDestroyChild(Widget* w);
		void removeAndDestroyChild(const std::string& widgetName);
		bool resizingAllowed();
		/**
		* Manually set the Dimensions of the widget.
		*/
		void setDimensions(const Rect& pixelDimensions);
		/**
		* This function specifies the widget that will be moved when the widget is *dragged*.
		* By default, the Dragging Widget is the widget itself, but this allows for the possibility
		* of moving a window by *dragging* the titlebar, or even the titlbar's text widget.
		*/
		void setDraggingWidget(Widget* w);

		virtual void setFont(const std::string& fontScriptName, bool recursive = false);
		/**
		* Allows clicking on a widget to not change the active widget.
		*/
		void setGainFocusOnClick(bool gainFocus);
		/**
		* Manually set mGrabbed to true.
		*/
		void setGrabbed(bool grabbed);
		virtual void setHeight(float pixelHeight);
		/**
		* If set to true, this widget will be hidden when its parent's widget is hidden.
		* NOTE: All widgets have this set to true by default.
		*/
		void setHideWithParent(bool hide);
		void setHorizontalAnchor(HorizontalAnchor a);
		void setInheritOpacity(bool inherit);
		void setInheritQuadLayer(bool inherit);
		/**
		* If set to false, widget cannot be moved.
		*/
		void setMovingEnabled(bool enable);
		/**
		* Manipulates the offset used to determine this widgets zOrder in rendering.
		*/
		virtual void setOffset(int offset);
		void setOpacity(float opacity);
		/**
		* Manually set position of widget.
		* NOTE: the values given are relative to the parent's top left corner, and not the screen!  For screen positioning,
		*  user the setScreenPosition function.
		*/
		virtual void setPosition(const float& pixelX, const float& pixelY);
		virtual void setPosition(const Point& pixelPoint);
		void setPropagateEventFiring(Event e, bool propogate);
		virtual void setQuadLayer(Quad::Layer l);
		/**
		* Manually set position of widget.
		* NOTE: the values given are relative to the render windows's top left corner, and not the parent widget!
		*/
		void setScreenPosition(const float& pixelX, const float& pixelY);
		void setScreenXPosition(const float& pixelX);
		void setScreenYPosition(const float& pixelY);
		void setScrollPaneAccessible(bool accessible);
		virtual void setSkin(const std::string& skinName, bool recursive = false);
		virtual void setSkinComponent(const std::string& skinComponent);
		/**
		* Manually set size of widget.
		*/
		virtual void setSize(const float& pixelWidth, const float& pixelHeight);
		virtual void setSize(const Size& pixelSize);
		/**
		* If set to true, this widget will be shown when its parent's widget is shown.
		* NOTE: most widgets have this set to true by default. (Menu's are false by default)
		*/
		void setShowWithParent(bool show);
		/**
		* If set to true, borders will be created, provided the matching *.border.*.png textures exist.
		* If set to false, any borders that have been created will be destroyed.
		*/
		void setUseBorders(bool use);
		/*
		* Sets the thickness of the borders on this widget
		*/
		void setBorderThickness(float thickness);
		/*
		* Sets the overlap of the borders on this widget
		*/
		void setBorderOverlap(float overlap);
		/*
		* If set to true, mouse LEAVE and ENTER events will take into account the exact pixel
		* the cursor is over.
		*/
		void setUseTransparencyPicking(bool use, bool recursive = true);
		void setVerticalAnchor(VerticalAnchor a);
		virtual void setWidth(float pixelWidth);
		void setXPosition(float pixelX);
		virtual void setYPosition(float pixelY);
		/**
		* Sets mVisible to true.  Widgets should override this to implement how they handle
		* showing.
		*/
		virtual void show();
		void showSkin();
		/**
		* Function required for certain widgets/functions to function properly, ie TextBox and fade.
		*/
		virtual void timeElapsed(const float time);
		inline bool isUnderTiming() const {return mEnabled;};

		/*
		* Allows texture of widget to change. (behavior by default)
		*/
		void unlockTexture();
		/** Checks if this widget property are subject
		*   to modification upon time
		*/
		inline bool getUnderEffect() const { return mUnderEffect; }
		/** set widget flag underEffect to that if user takes control
		*   he knows that he has to disable effect before.
		*/
		void setUnderEffect(bool val) { mUnderEffect = val; }

	protected:
		virtual void setGUIManager(GUIManager* gm);
		virtual void setParent(Widget* parent);
		virtual void setQuadContainer(QuadContainer* container);

		// Positions/sizes the widget according to parent's size.
		virtual void _applyAnchors();
		void _deriveAnchorValues();
	protected:
		virtual ~Widget();

		GUIManager*					mGUIManager;
		std::string					mInstanceName;
		Type						mWidgetType;

		// PROPERTIES
		bool						mCanResize;
		bool						mDragXOnly;
		bool						mDragYOnly;
		std::string					mFontName;
		std::string					mSkinName;
		bool						mHideSkin;
		bool						mVisible;
		bool						mEnabled;
		bool						mGainFocusOnClick;
		bool						mGrabbed;
		bool						mTextureLocked;
		Quad::Layer					mQuadLayer;
		bool						mInheritQuadLayer;
		bool						mMovingEnabled;
		bool						mDraggingEnabled;
		std::string					mSkinComponent;
		bool						mScrollPaneAccessible;
		bool						mInheritOpacity;
		float						mOpacity;
		// number of parents iterated to get to sheet.
		int							mOffset;
		bool						mHideWithParent;
		bool						mShowWithParent;
		bool						mUseTransparencyPicking;

		Widget*						mParentWidget;
		Widget*						mWidgetToDrag;

		// Used for overTransparentPixel function
		SkinSet*					mSkinSet;

		// ANCHORS
		HorizontalAnchor			mHorizontalAnchor;
		VerticalAnchor				mVerticalAnchor;
		float						mPixelsFromParentRight;
		float						mPixelsFromParentBottom;

		// Implement the Enter/Leave functionality.
		bool						mEntered;

		// state that this widget property are subject
		// to modification upon time
		bool						mUnderEffect;

		Quad*						mQuad;
		// All widgets have at least 1 quad, but can use more.
		QuadArray					mQuads;
		Quad* _createQuad();

		// Keeping track of the QuadContainer this Quad belongs to.
		QuadContainer*				mQuadContainer;

		Widget*						_createChild(const std::string& name, Type t);
		// List of any child widgets this widget may have.
		WidgetArray					mChildWidgets;

		virtual Widget*				_createComponent(const std::string& name, Type t);
		bool						_hasComponent(const std::string& name);
		WidgetArray					mComponents;

		// Pixel position relative to parent.  (0,0) is the Parent Widgets top Left corner.
		Point						mPosition;
		// Used for scrolling widgets.
		Point						mScrollOffset;
		// Size in pixels.
		Size						mSize;

		bool						mUseBorders;
		void						_createBorders();
		void						_destroyBorders();
		float						mBorderThickness;
		float						mBorderOverlap;

		// Event handlers! One List per event per widget
		EventHandlerArray mUserEventHandlers[EVENT_END_OF_LIST];
		bool mPropogateEventFiring[EVENT_END_OF_LIST];

		EventHandlerArray mEventListeners;

		void _initEventHandlers();

		virtual void _setScrollXOffset(float pixelXOffset);
		virtual void _setScrollYOffset(float pixelYOffset);
	protected:
		virtual void onPositionChanged(const EventArgs& args);
		virtual void onSizeChanged(const EventArgs& args);
	};
}

#endif
