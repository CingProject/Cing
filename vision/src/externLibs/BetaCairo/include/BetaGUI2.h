#ifndef BETAGUI2
#define BETAGUI2

#include "Ogre.h"
#include "BetaCairo.h"

namespace BetaGUI {
	
	class Skin;
	class GUI;
	class Window;
	class EventListener;
	class Widget;
	class Button;
	class TextInput;
	
	struct Coords {
		Ogre::uint x;
		Ogre::uint y;
		Ogre::uint w;
		Ogre::uint h;

		Coords() : x(0), y(0), w(0), h(0) {}
		Coords(Ogre::uint X,Ogre::uint Y) : x(X), y(Y), w(0), h(0) {}
		Coords(Ogre::uint X,Ogre::uint Y,Ogre::uint W,Ogre::uint H) : x(X), y(Y), w(W), h(H) {}
		bool inside(Coords c) {return((c.x >= x && c.y >= y) && (c.x <= x + w && c.y <= y + h));}
		bool inside(Coords c, Coords add) {return((c.x >= add.x + x && c.y >= add.y + y) && (c.x <= add.x + x + w && c.y <= add.y + h));}

	};

	enum WindowTypes { 
		NONE=0,
		MOVE=1,
		RESIZE=2,
		RESIZE_AND_MOVE=3
	};

	enum ButtonTypes {
		PUSH,
		HOLD
	};

	enum TextInputTypes {
		SINGLE_LINE = 0,
		MULTI_LINE = 1,
		NUMERIC = 2
	};

	typedef std::string		String;
	typedef std::string		Caption;
	//typedef std::wstring	Caption;
	

	namespace Colour {
		Ogre::ColourValue fromRGB(Ogre::uint r, Ogre::uint g, Ogre::uint b);
		Ogre::ColourValue fromRGBA(Ogre::uint r, Ogre::uint g, Ogre::uint b, Ogre::uint a);
		Ogre::ColourValue fromHSV(float hue, float saturation, float value, float alpha = 1.0f);
	}

	class Theme {
	
		public:

			virtual float getWidgetPadding(unsigned char t) = 0;
			virtual void drawWindow(BetaGUI::Window *window, BetaCairo::Drawing *mDrawing, BetaGUI::Coords c) = 0;
			virtual void drawButton(BetaGUI::Button *button, BetaCairo::Drawing *mDrawing, BetaGUI::Coords c) = 0;
			virtual void drawTextInput(BetaGUI::TextInput *textinput, BetaCairo::Drawing *mDrawing, BetaGUI::Coords c) = 0;
	
	};

	class EventListener {

		public:
	
			// Window has moved
			virtual void onWindowMove(BetaGUI::Window *window) {}
			
			// Window has resized
			virtual void onWindowResize(BetaGUI::Window *window) {}
			
			// A Widget has been selected
			virtual void onWidgetSelect(BetaGUI::Widget *widget) {}
			
			// A widget has had a confirmed keypress (an enter)
			virtual void onWidgetConfirm(BetaGUI::Widget *widget) {}
	
	};

	class GUI {

		friend class Window;

		public:
			
			GUI(String BaseOverlay);
			~GUI();

			Window*		createWindow(String name, Coords coords, WindowTypes type, EventListener *listener, Theme *theme);
			void		destroyWindow(String name);
			void		createMousePointer(Coords coords);
			void		injectMouseMovement(Coords coords, bool select);

			// Inject a keypress.
			// @param key - ASCII Key (1 character, prefered). To insert a backspace use \b, to insert a carriage return insert \n
			//
			void		injectKeyPress(Ogre::String key);
	
		
		protected:

			std::map<String, Window*>	mWindow;
			Ogre::Overlay*				mOverlayContainer;
			Ogre::OverlayContainer*		mMousePointer;
			BetaCairo::Drawing*			mMousePointer_Texture;

			Ogre::OverlayContainer*		createOverlay(Ogre::String name, Ogre::String material, Coords c);
		

		private:

	};

	class Window {
		
		friend class GUI;
		friend class Widget;
		friend class Theme;
		
		public:

			Button*	createButton(String name, Caption caption, Coords coords, ButtonTypes type);
			TextInput* createTextInput(String name, Caption caption, Coords coords, unsigned int maxCharacters, TextInputTypes types);
		
			Theme*	getTheme()	{return mTheme;}
			String	getName()	{return mName;}
			Coords	getCoordinates() {return mCoords;}
			Caption getCaption() {return mCaption;}
			WindowTypes getType() {return mType;}
			void	hideGroup(unsigned int g);
			void	showGroup(unsigned int g);
			void setCoordinates(Coords coords);
			void setCaption(Caption caption);
	
			Widget* getWidget(Ogre::String name);

			std::map<String, Widget*>	mWidgets;
		protected:

			Window(String name, Coords coords, WindowTypes type, EventListener *listener, Theme *theme, GUI *gui);
			~Window();
			
			bool Check(Coords coords, bool select);
			bool Check(Ogre::String character);

			String						mName;
			WindowTypes					mType;
			Coords						mCoords;
			Caption						mCaption;
			Ogre::OverlayContainer*		mOverlayContainer;
			BetaCairo::Drawing*			mDrawing;
			Ogre::MaterialPtr			mMaterial;
			
			EventListener*				mEventListener;
			Theme*						mTheme;

		private:
			
			GUI							*mGUI;
	};

	class Widget {
		
		friend Window;
	
		public:

			String	getName()			{return mName;}
			Caption getCaption()		{return mCaption;}
			bool	getHoverState()		{return mHover;}
			bool	getSelectedState()	{return mSelected;}
			char	getType()			{return mType;}
			void	setGroup(unsigned char g) {mGroup = g;}
			unsigned int getGroup() {return mGroup;}
			void	hide();
			void	show();

			virtual bool		check(Coords coords, bool select) = 0;
			virtual bool		check(Ogre::String s) = 0;

		protected:
			
			Widget(String name, Caption caption, Coords coords, Window *window, unsigned char type): 
				 mName(name), mCaption(caption), mCoords(coords), mWindow(window), mDrawing(mWindow->mDrawing), mType(type), mSelected(false), mHover(false) {}
			~Widget() {}
		
			char				mType;

			String				mName;
			Caption				mCaption;
			Coords				mCoords;
			Window				*mWindow;
			BetaCairo::Drawing	*mDrawing;
			bool				mHover;
			bool				mSelected;
			unsigned int		mGroup;
			
			void setBounds(Ogre::String text, float padding = 1.5f);

	};

	class Button : public Widget {
		
		friend Window;

		public:

			
			bool check(Coords coords, bool select);
			bool check(Ogre::String s);


		protected:

			Button(String name, Caption caption, Coords coords, ButtonTypes type, Window *window);
			~Button();
			
			bool getValue() {
				return mPressed;
			}
	
		
			ButtonTypes mType;
			bool		mPressed;

			static const char _type;
	};

	class TextInput : public Widget {

		friend Window;

	protected:

		TextInput(String name, Caption caption, unsigned int maxCharacters, Coords coords, TextInputTypes types, Window *window);
		~TextInput();

	public:

		bool check(Coords coords, bool select);
		bool check(Ogre::String s);

		
		static const char _type;
	};

} // End of Namespace
#endif