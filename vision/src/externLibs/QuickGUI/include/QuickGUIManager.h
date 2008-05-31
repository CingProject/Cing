#ifndef QUICKGUIMANAGER_H
#define QUICKGUIMANAGER_H

#include "OgrePrerequisites.h"
#include "OgrePlatform.h"
#include "OgreRenderQueueListener.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUISkinSetManager.h"
#include "QuickGUIKeyCode.h"
#include "QuickGUIMouseButtonID.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <time.h>
#include <utility>
#include <ctype.h>
#include <vector>

namespace QuickGUI
{
	class Effect;

	/** Manages Windows, Mouse Cursor, and Input
		@remarks
		The most important class of QuickGUI, responsible for creating and
		destroying Windows, updating the Mouse Cursor, and handling input.
		GUIManager has a simple clearAll method, which cleans up all created
		widgets.  This supports multiple game states that have differing GUI
		Layouts.
		@note
		GUIManager is a Singleton, and frequently accessed by widgets, for
		information on the rendering window, default font, text color, character
		height, or setting focus to itself. (Window Widget does this)
		@note
		GUIManager allows 5 zOrder per window, and manages Window zOrdering so
		that windows and widgets are rendered properly on top of each other.
	*/
	class _QuickGUIExport GUIManager :
		public Ogre::RenderQueueListener
	{
	public:
		friend class ComboBox;
		friend class Widget;
		friend class Sheet;
		friend class Root;
	public:
		void _notifyViewportDimensionsChanged();

		// Keep track of effect so that manager can update them.
		void addEffect(Effect* e);

		/**
		* Iterates through Window List and destroys it, which properly destroys all child widgets.
		*/
		void clearAll();

		Sheet* createSheet();
		Sheet* createSheet(const std::string& name);

		/** Destroys a Window and all child widgets that exist
		    @param
				name Name of the window to destroy.
			@note
				name can also be reference name given to the window.
			@note 
				no exception is thrown if window does not exist
		*/
		void destroySheet(const std::string& name);
		/** Destroys a Window and all child widgets that exist
		    @param
				window Window to destroy.
		*/
		void destroySheet(Sheet* sheet);

		/**
		* Stores a reference to the widget in the free list, which will be destroyed
		* next frame.
		*/
		void destroyWidget(Widget* w);
		void destroyWidget(const std::string& widgetName);

		/**
		* Returns the sheet currently being used, whether shown or hidden.
		*/
		Sheet* getActiveSheet();
		Widget* getActiveWidget();
		std::string getDebugString();
		/**
		* Returns the default sheet, automatically created with the GUI manager.
		*/
		Sheet* getDefaultSheet();
		bool getDetermineClickEvents();

		MouseCursor* getMouseCursor();
		Widget* getMouseOverWidget();
		std::string getName();
		/**
		* Get viewport all widgets of this manager are rendering to.
		*/
		Ogre::Viewport* getViewport();
		/**
		* Get primary render window width in pixels
		*/
		float getViewportWidth();
		/**
		* Get primary render window height in pixels
		*/
		float getViewportHeight();

		/**
		* Iterates through sheet list and returns the Sheet with the
		* matching name.  Null if no match found.
		*/
		Sheet* getSheet(const std::string& name);

		std::string generateName(Widget::Type t);

		/**
		* Useful for Text Input Widgets, like the TextBox
		*/
		bool injectChar(Ogre::UTFString::unicode_char c);
		bool injectKeyDown(const KeyCode& kc);
		bool injectKeyUp(const KeyCode& kc);

		bool injectMouseButtonDown(const MouseButtonID& button);
		bool injectMouseButtonUp(const MouseButtonID& button);
		bool injectMouseClick(const MouseButtonID& button);
		bool injectMouseDoubleClick(const MouseButtonID& button);
		bool injectMouseTripleClick(const MouseButtonID& button);
		/**
		* Injection when the mouse leaves the primary render window
		*/
		bool injectMouseLeaves(void);
		bool injectMouseMove(const int& xPixelOffset, const int& yPixelOffset);
		bool injectMousePosition(const int& xPixelPosition, const int& yPixelPosition);
		bool injectMouseWheelChange(float delta);
		void injectTime(float time);

		bool isKeyModifierDown(KeyModifier k);
		/**
		* Checks if the desired widget name already exists.  If it already exists,
		* false is returned.
		*/
		bool isNameUnique(const std::string& name);

		void notifyNameFree(const std::string& name);
		void notifyNameUsed(const std::string& name);

		void registerTimeListener(Widget* w);

		virtual void renderQueueStarted(Ogre::uint8 id, const std::string& invocation, bool& skipThisQueue);
		virtual void renderQueueEnded(Ogre::uint8 id, const std::string& invocation, bool& repeatThisQueue);

		void removeFromRenderQueue();

		/**
		* Sets the active sheet, displaying it on screen.
		* NOTE: You do not and should not hide previously used sheets before making this call.
		*  Hiding the previous sheet will hide all widgets and store it as hidden.  This call
		*  switches sheets used for rendering, thus, it does not affect widgets mVisible variable.
		*  The benefit is that you preserve Sheet state.
		*/
		void setActiveSheet(Sheet* s);
		/**
		* Activates the widget w, and deactivates the previously active widget. (if exists)
		*/
		void setActiveWidget(Widget* w);
		void setDebugString(const std::string s);
		void setDetermineClickEvents(bool determine);
		/*
		* Sets the Render Queue Group to render on.  By default, this is RENDER_QUEUE_OVERLAY.
		*/
		void setRenderQueueID(Ogre::uint8 id);
		/*
		* Hook into this SceneManager's render queue to render its quads to the screen.  By default,
		* the first Scene Manager instance is used.
		*/
		void setSceneManager(Ogre::SceneManager* sm);
		/*
		* Set the list of code points that will be accepted by the injectChar function.  English code points 9,32-166
		* are supported by default.
		*/
		void setSupportedCodePoints(const std::set<Ogre::UTFString::code_point>& list);
		/*
		* Sets the viewport all widgets of this manager will render to.
		*/
		void setViewport(Ogre::Viewport* vp);

		void unregisterTimeListener(Widget* w);

	protected:
		std::string			mName;
		// Viewport which renders all widgets belonging to this manager.
		Ogre::Viewport*			mViewport;

		std::string			mDefaultSkin;

		// required to hook into the RenderQueue
		Ogre::SceneManager*		mSceneManager;
		// ID of the queue that we are hooked into
		Ogre::uint8				mQueueID;

		MouseCursor*			mMouseCursor;

		std::set<std::string>	mWidgetNames;

		// range of supported codepoints used for injectChar function.
		std::set<Ogre::UTFString::code_point> mSupportedCodePoints;

		QuickGUI::Sheet*		mDefaultSheet;
		// Sheet currently being shown.
		QuickGUI::Sheet*		mActiveSheet;
		// Includes the Default Sheet.
		std::list<Sheet*>		mSheets;

		std::string			mDebugString;

		void					_destroyWidget(Widget* w);
		// list of widgets to delete on next frame.
		WidgetArray	mFreeList;

		bool					mDetermineClickEvents;
		// timer used to get time readings.
		Ogre::Timer*			mTimer;
		// Maximum number of milliseconds a click can be performed in.
		unsigned long			mClickTime;				// time from mouse down to mouse up
		unsigned long			mDoubleClickTime;		// time from mouse click to mouse down
		unsigned long			mTripleClickTime;		// time from mouse double click to mouse down
		// Store the last time a click was performed.
		unsigned long			mTimeOfButtonDown[NUM_MOUSE_BUTTONS];
		unsigned long			mTimeOfClick[NUM_MOUSE_BUTTONS];
		unsigned long			mTimeOfDoubleClick[NUM_MOUSE_BUTTONS];

		// Keep track of mouse button down/up and on what widget.  This prevents left mouse button down on button A,
		// moving the mouse to button B, and releasing the left mouse button, causing button B to be pressed. (example)
		Widget*					mMouseButtonDown[NUM_MOUSE_BUTTONS];

		Widget*					mWidgetContainingMouse;
		// Stores reference to last clicked Widget.
		Widget*					mActiveWidget;

		bool					mDraggingWidget;

		// Keep track of open menu's, for mouse detection.
		std::set<Widget*>	mOpenMenus;
		void _menuOpened(Widget* w);
		void _menuClosed(Widget* w);

		// Keep track of effect, to update.
		std::list<Effect*>		mActiveEffects;

		WidgetArray	mTimeListeners;

		SkinSetManager*			mSkinSetManager;

		//! Bit field that holds status of Alt, Ctrl, Shift
		unsigned int			mKeyModifiers;
	protected:
		/** Constructor */
		GUIManager(const std::string& name, Ogre::Viewport* vp, const std::string& defaultSkin);
		/** Standard Destructor. */
		~GUIManager();
    };
}

#endif
