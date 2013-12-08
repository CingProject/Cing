/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef _FontManager_H_
#define _FontManager_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"
#include "FontProperties.h"
#include "MovableText.h"

// Common
#include "common/Singleton.h"

namespace Cing
{
	

/**
 * @internal
 * Manages the font system
 */
class FontManager: public Singleton< FontManager >
{

public:

	// Required for singleton
	friend class Singleton< FontManager >;

	// Destructor
	virtual ~FontManager();

	// Init / Release / Draw
	bool                    init		();
	void                    end			();
	void                    postRender  ();

	// Set methods
	void					setActiveFont				( Font& font );
	void					addText						();
	void					updateActiveFontProperties	();

  	// Query methods
	FontProperties&				getActiveFontProperties	() { return m_activeFontProperties; }
	bool						isValid					() const { return m_bIsValid && m_activeFont; }
	float						textWidth				( const std::string& text );
	float						textHeight				( const std::string& text, float textBoxWidth );
	int							textLineBreakCount		( const std::string& text, float textBoxWidth );
	float						characterHeight			();
	float						spaceWidth				();
	std::vector<std::string>	splitInLines			( const std::string& str, float textBoxWidth );
	
private:

	// private types
	typedef std::vector< MovableText* > TextList;

	// private constructor to ensure singleton
	FontManager      ();

	// Internal methods
	MovableText*	getNewText();

	// Attributes
	FontProperties				m_activeFontProperties;	///< Font properties used in the next call to text to render a string of text
	Font*						m_activeFont;			///< Active font (activated calling textFont function)
	Font						m_defaultFont;			///< Font to use in case the user does not want to create one...
	TextList					m_activeFontsToRender;	///< Text areas to be rendered in the next draw call
	size_t						m_nextFontToUse;		///< Index to reuse previously used fonts
	bool						m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called
	
};

} // namespace Cing

#endif // _FontManager_H_
