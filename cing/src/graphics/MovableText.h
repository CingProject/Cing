/**
* File: MovableText.h
*
* description: This create create a billboarding object that display a text.
*
* @author	2003 by cTh see gavocanov@rambler.ru
* @update	2006 by barraq see nospam@barraquand.com
* @update	2009 by Cing team for the Cing project
*/

#ifndef _MovableText_H__
#define _MovableText_H__

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"
#include "Color.h"
#include "Font.h"


// common
#include "common/eString.h"
#include "common/CommonTypes.h"

// Ogre
#include "Overlay/OgreFont.h"
#include "Overlay/OgreOverlayElement.h"
#include "OgreMovableObject.h"




#define POS_TEX_BINDING			0
#define COLOUR_BINDING			1
#define UNICODE_NEL				0x0085
#define UNICODE_CR				0x000D
#define UNICODE_LF				0x000A
#define UNICODE_SPACE			0x0020
#define UNICODE_ZERO			0x0030
#define UNICODE_TAB				0x0009
#define DEFAULT_INITIAL_CHARS	12



namespace Cing
{

	class MovableText : public Ogre::MovableObject, public Ogre::Renderable
	{
	public:
		// Constructor / Destructor
		MovableText();
		virtual ~MovableText();

		// Init / End
		void init	( Ogre::SceneNode* parentNode = NULL /*const String& caption, const String& fontName = Font::DEFAULT_FONT_NAME, float charHeight = 1.0, const Color& color = Color::White*/);
		void end	();

		// Set settings
		void    setFontName					(const String &fontName);
		void    setCaption					(const Ogre::DisplayString& caption);
		void    setText						(const Ogre::DisplayString& text) { setCaption(text); }
		void    setColor					(const Color& color);
		void    setCharacterHeight			(float height);
		void    setSpaceWidth				(float width);
		void	setWordWrap					(bool wordWrap) { mwordWrap = wordWrap; }
		void    setTextAlignment			(int horizontalAlignment, int verticalAlignment);
		void	setLeading					( float leading ) { m_leading = leading; }
		void    setPosition					( float x, float y );
		void    setPosition					( float x, float y, float z );
		void    setPosition					( const Vector& pos ) { setPosition( pos.x, pos.y, pos.z ); }
		void	setRotation					( const Vector& rotation );
		void	setRotation					( const Quaternion& quat );
		void	setScale					( float x, float y );
		void	setScale					( float x, float y, float z );
		void	setScale					( const Vector& scale ) { setScale (scale.x, scale.y, scale.z); }
		void	show						( bool show = true );
		void    showOnTop					( bool show = true );
		void	setAlwaysFaceCamera			( bool alwaysFaceCamera ) { mAlwaysFaceCamera = alwaysFaceCamera; }
		void	setTextAreaWidth			( float width );
		void	setTextAreaHeight			( float height );
		void	setBackgroundTransparency	( float transparency );

		// Get settings
		bool            				isValid					() const	{ return m_bIsValid;   }
		const   String					&getFontName			() const 	{ return mFontName; }
		const   Ogre::DisplayString		&getCaption				() const 	{ return mCaption; }
		const   Color					&getColor				() const 	{ return mColor; }
		float							getCharacterHeight		() const 	{ return mCharHeight; }
		float							getSpaceWidth			() const 	{ return mSpaceWidth; }
		bool							getWordWrap				() const 	{ return mwordWrap; }
		bool							getShowOnTop			() const 	{ return mOnTop; }
		bool							getAlwaysFaceCamera		() const	{ return mAlwaysFaceCamera; }
		AABox							getAABB					()			{ return mAABB; }
		Ogre::Node*						getNode					()			{ return mParentNode; }
		const Vector&					getScale				() const	{ return mScale; }

		// Calculators
		float							getTextBlockHeightPixels( const std::string& text, float textBlockWidth );

		bool							isVisible				() const	{ return MovableObject::isVisible(); }

		// Helpers
		static	inline bool	IsTab			(Ogre::Font::CodePoint c) { return c == UNICODE_TAB; }
		static	inline bool	IsSpace			(Ogre::Font::CodePoint c) { return c == UNICODE_SPACE; }
		static	inline bool	IsNewLine		(Ogre::Font::CodePoint c) { return c == UNICODE_CR || c == UNICODE_LF || c == UNICODE_NEL; }
		static	inline bool	IsWhiteSpace	(Ogre::Font::CodePoint c) { return IsTab(c) || IsSpace(c) || IsNewLine(c); }
		static	inline bool	IsCRLF			(Ogre::Font::CodePoint a, Ogre::Font::CodePoint b) { return a == UNICODE_CR && b == UNICODE_LF; }

		// Add to build on Shoggoth (Ogre)
		virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);

	// protected methods and overload
	//protected:

		// from MovableText, create the object
		void	_setupGeometry					();
		void	checkMemoryAllocation			( size_t numChars );
		void	_updateColors					();

		// from MovableObject
		void						getWorldTransforms	(Matrix4 *xform) const;
		float   					getBoundingRadius	(void) const {return mRadius;};
		float   					getSquaredViewDepth	(const Ogre::Camera *cam) const {return 0;};
		const   Ogre::Quaternion&	getWorldOrientation	(void) const;
		const   Vector&				getWorldPosition	(void) const;
		const   AABox&				getBoundingBox		(void) const {return mAABB;};
		const   String&				getName				(void) const {return mName;};
		const   String&				getMovableType		(void) const {static String movType = "MovableText"; return movType;};

		void    _notifyCurrentCamera(Ogre::Camera *cam);
		void    _updateRenderQueue	(Ogre::RenderQueue* queue);

		// from renderable
		void						getRenderOperation(Ogre::RenderOperation &op);
		const   Ogre::MaterialPtr&	getMaterial(void) const { return mpMaterial; };
		const   Ogre::LightList&	getLights(void) const {return mLList;};


	// Private stuff
	private:

		// internal types
		typedef std::list<int> ForcedLineBreaks;

		// internal methods
		void	calculateLineBreaks	( ForcedLineBreaks& forcedLineBreaks );
		float	getWordLength		( Ogre::DisplayString::iterator it, const Ogre::DisplayString::iterator& end  );
		void	advanceWord			( Ogre::DisplayString::iterator& it, const Ogre::DisplayString::iterator& end  );
		void	calculateLineLeft	( const Ogre::DisplayString::iterator& currentIt, const Ogre::DisplayString::iterator& iend, const ForcedLineBreaks& forcedLineBreaks, float& left, float& lineLength );
		void	drawCharacter		( float*& pVert, const Ogre::Font::CodePoint& character, float& top, float& left, float& lineLength );
		void	set2dRendering		();
		void	set3dRendering		();


		// Static attributes
		static long					count; ///< Used to create unique names


		// Attributes
		String						mFontName;
		String						mType;
		String						mName;
		Ogre::DisplayString			mCaption;
		int							mHorizontalAlignment;
		int							mVerticalAlignment;
		float						m_leading; // Distance in pixels between different text lines

		Color						mColor;
		Ogre::RenderOperation		mRenderOp;
		AABox						mAABB;
		Ogre::LightList				mLList;

		float						mCharHeight;
		float						mSpaceWidth;
		float						m_textAreaWidth, m_textAreaHeight;

		bool						mNeedUpdate;
		bool						mUpdateColors;
		bool						mOnTop;

		float						mTimeUntilNextToggle;
		float						mRadius;
		Vector						mScale;
		Matrix3						mLocalRotation;

		Ogre::Camera				*mpCam;
		Ogre::RenderWindow			*mpWin;
		Ogre::FontPtr				mpFont;
		Ogre::MaterialPtr			mpMaterial;
		Ogre::MaterialPtr			mpBackgroundMaterial;

		// for calculation of AABB
		Vector						m_min, m_max;
		float						m_maxSquaredRadius;

        size_t						mAllocSize;
		float						mViewportAspectCoef;
		bool						mwordWrap;
		bool						mAlwaysFaceCamera;
		bool						mRender2D;
		bool						m_bIsValid;



	};

}

#endif
