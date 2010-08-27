/**
 * File: MovableText.cpp
 *
 * description: This create create a billboarding object that display a text.
 *
 * @author	2003 by cTh see gavocanov@rambler.ru
 * @update	2006 by barraq see nospam@barraquand.com
 * @update	2009 by Cing team for the Cing project
 */

#include "MovableText.h"

// Common
#include "common/CommonUtilsIncludes.h"
#include "common/CommonConstants.h"
#include "common/CommonTypes.h"

// Ogre
#include "Ogre.h"
#include "OgreFontManager.h"

// Graphics
#include "graphics/GraphicsManager.h"

namespace Cing
{
long MovableText::count = 0;

MovableText::MovableText() :
	mpCam(NULL)
	, mpWin(NULL)
	, mpFont(NULL)
	, mName(""), mCaption(""), mFontName(""), mCharHeight(Font::DEFAULT_FONT_SIZE), mColor(Color::White), mType("MovableText"),
			mTimeUntilNextToggle(0), mSpaceWidth(0), mUpdateColors(true), mOnTop(false), mHorizontalAlignment(LEFT),
			mVerticalAlignment(TOP), m_textAreaWidth(-1),
			m_textAreaHeight(-1), mAllocSize(0), mwordWrap(true), mAlwaysFaceCamera(false), mRender2D(false), m_bIsValid(false),
			m_maxSquaredRadius(-1), mScale( Vector::UNIT_SCALE )
{

	// Create unique name
	mName = "MovableText_" + intToString(count++);

}

MovableText::~MovableText()
{
	end();
}

void MovableText::init( Ogre::SceneNode* parentNode /*= NULL*/ /*const String &caption, const String &fontName, float charHeight, const Color &color*/)
{
	// Only init once
	if ( !isValid() )
	{
		// Julio: From Ogre
		// Set up the render op
		// Combine positions and texture coords since they tend to change together
		// since character sizes are different
		mRenderOp.vertexData = OGRE_NEW Ogre::VertexData();
		Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
		size_t offset = 0;
		// Positions
		decl->addElement(POS_TEX_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		// Texcoords
		decl->addElement(POS_TEX_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
		// Colours - store these in a separate buffer because they change less often
		decl->addElement(COLOUR_BINDING, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOp.useIndexes = false;
		mRenderOp.vertexData->vertexStart = 0;

		// Note: Vertex buffer will be created in checkMemoryAllocation
		checkMemoryAllocation(DEFAULT_INITIAL_CHARS);

		// Attatch to parent scene node
		if ( parentNode == NULL )
			parentNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode();
		parentNode->attachObject( this );

		// Set default font for now
		setFontName(Font::DEFAULT_FONT_NAME);

		// On top by default
		showOnTop(true);

		m_bIsValid = true;
	}

	// Reset vars
	mLocalRotation = Matrix3::IDENTITY;
}

void MovableText::end()
{
	// Already released?
	if ( !isValid() )
		return;

	if ( mRenderOp.vertexData )
		Release(mRenderOp.vertexData);

	// May cause crashing... check this and comment if it does
	if ( !mpMaterial.isNull() )
	{
		Ogre::MaterialManager::getSingletonPtr()->remove(mpMaterial->getName());
		mpMaterial.setNull();
	}

	m_bIsValid = false;
}

void MovableText::setFontName( const String &fontName )
{
	// Julio: from ogre
	mpFont = Ogre::FontManager::getSingleton().getByName(fontName);
	if ( mpFont.isNull() )
	{
		LOG_ERROR( "MovableText::setFontName: Could not find font %s", fontName.toChar() );
	}
	mpFont->load();
	mpMaterial = mpFont->getMaterial();
	mpMaterial->setDepthCheckEnabled(false);
	mpMaterial->setLightingEnabled(false);
	mpMaterial->setCullingMode( Ogre::CULL_NONE );
			
	
	// set 2d rendering by default
	set2dRendering();


	mNeedUpdate = true;
}

void MovableText::setCaption( const Ogre::DisplayString &caption )
{
	if ( caption != mCaption )
	{
		mCaption = caption;
		mNeedUpdate = true;
	}
}

void MovableText::setColor( const Color &color )
{
	if ( color != mColor )
	{
		mColor = color;
		mUpdateColors = true;
	}
}

void MovableText::setCharacterHeight( float height )
{
	if ( height != mCharHeight )
	{
		mCharHeight = height;
		mNeedUpdate = true;
	}
}

void MovableText::setSpaceWidth( float width )
{
	if ( width != mSpaceWidth )
	{
		mSpaceWidth = width;
		mNeedUpdate = true;
	}
}

void MovableText::setTextAreaWidth( float width )
{
	if ( width != m_textAreaWidth )
	{
		m_textAreaWidth = width;
		mNeedUpdate = true;
	}
}

void MovableText::setTextAreaHeight( float height )
{
	if ( height != m_textAreaHeight )
	{
		m_textAreaHeight = height;
		mNeedUpdate = true;
	}
}

void MovableText::setBackgroundTransparency( float transparency )
{
	// Semitransparent background for the text
	mpMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(	Ogre::LBX_ADD_SIGNED,
																						Ogre::LBS_TEXTURE, 
																						Ogre::LBS_MANUAL, 
																						transparency);
}


void MovableText::setPosition( float x, float y )
{
	// Calculate position depending on the current camera setting
	if ( GraphicsManager::getSingleton().isProcessingMode() )
	{
		y = height - y;
		getParentNode()->setScale( getParentNode()->getScale() * Vector(1, -1, 1) );
	}

	float _2dXPos = (x / (float)width) * 2.0f - 1;
	float _2dYPos = -((y / (float)height) * 2.0f - 1);
	getParentNode()->setPosition( _2dXPos, _2dYPos, 0 );

	// set 2D rendering
	set2dRendering();
}


void MovableText::setPosition( float x, float y, float z )
{		
	getParentNode()->setPosition( x, y, z );

	// set 3d rendering
	set3dRendering();
}


void MovableText::setTextAlignment( int horizontalAlignment, int verticalAlignment )
{
	if ( mHorizontalAlignment != horizontalAlignment )
	{
		mHorizontalAlignment = horizontalAlignment;
		mNeedUpdate = true;
	}
	if ( mVerticalAlignment != verticalAlignment )
	{
		mVerticalAlignment = verticalAlignment;
		mNeedUpdate = true;
	}
}

void MovableText::setRotation( const Vector& rotation )
{
	// Build quaterniotn to build then rot matrix
	Quaternion q(&rotation);
	q.ToRotationMatrix(mLocalRotation);
}

void MovableText::setRotation( const Quaternion& quat )
{
	quat.ToRotationMatrix(mLocalRotation);
}

void MovableText::setScale( float x, float y )
{
	if ( GraphicsManager::getSingleton().isProcessingMode() )
		y *= -1;

	mScale.x = x;
	mScale.y = y;
	mScale.z = 1;
	getParentNode()->setScale( mScale );
}



void MovableText::setScale( float x, float y, float z )
{
	if ( GraphicsManager::getSingleton().isProcessingMode() )
		y *= -1;

	mScale.x = x;
	mScale.y = y;
	mScale.z = z;
	getParentNode()->setScale( mScale );
}

void MovableText::show( bool show /*= true*/)
{
	assert(mParentNode);

	// We assume the parent node is a scence node
	static_cast<Ogre::SceneNode*> (mParentNode)->setVisible(show);
}

void MovableText::showOnTop( bool show )
{
	if ( mOnTop != show && !mpMaterial.isNull() )
	{
		mOnTop = show;
		mpMaterial->setDepthBias(1.0, 1.0);
		mpMaterial->setDepthCheckEnabled(!mOnTop);
		mpMaterial->setDepthWriteEnabled(mOnTop);
	}
}

void MovableText::checkMemoryAllocation( size_t numChars )
{
	// Check if we need a bigger buffer
	if ( mAllocSize < numChars )
	{
		// Create and bind new buffers
		// Note that old buffers will be deleted automatically through reference counting

		// 6 verts per char since we're doing tri lists without indexes
		// Allocate space for positions & texture coords
		Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
		Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

		mRenderOp.vertexData->vertexCount = numChars * 6;

		// Create dynamic since text tends to change alot
		// positions & texcoords
		Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(POS_TEX_BINDING), mRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		bind->setBinding(POS_TEX_BINDING, vbuf);

		// colours
		vbuf = Ogre::HardwareBufferManager::getSingleton(). createVertexBuffer(decl->getVertexSize(COLOUR_BINDING),
				mRenderOp.vertexData->vertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		bind->setBinding(COLOUR_BINDING, vbuf);

		mAllocSize = numChars;

		// force color buffer regeneration
		mUpdateColors = true;
	}

}

void MovableText::_setupGeometry()
{
	// Check we have the font
	if ( mpFont.isNull() )
	{
		// not initialised yet, probably due to the order of creation in a template
		return;
	}

	// Reset vars (used to know which is the first tri in relation to the bbox calculation
	m_maxSquaredRadius = -1;

	// Check memory allocation for the current caption
	size_t charlen = mCaption.size();
	checkMemoryAllocation(charlen);

	// Number of vertext for the current text
	mRenderOp.vertexData->vertexCount = charlen * 6;

	// Get position / texcoord buffer
	Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(POS_TEX_BINDING);
	float *pVert = static_cast<float*> (vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	// Cing
	mViewportAspectCoef = 1.0f;

	// Starting point
	float left = 0 * 2.0 - 1.0;
	float top = -((0 * 2.0) - 1.0);

	// Derive space with from a number 0
	// Cing note: widht half of the width ogre was using (do to testing, it looks better this way)
	if ( mSpaceWidth == 0 )
	{
		mSpaceWidth = mpFont->getGlyphAspectRatio(UNICODE_ZERO) * mCharHeight;// * 2.0 * mViewportAspectCoef;
	}

	// Calculate vertical offset depending on the vertical alignment
	float verticalOffset = 0;
	bool loaded = mpFont->isLoaded();
	switch (mVerticalAlignment)
	{
	case BOTTOM:
		verticalOffset = mCharHeight;
	break;
	case CENTER:
		verticalOffset = 0.5f * mCharHeight;
	break;
	case TOP:
		verticalOffset = 0.0f;
	break;
	}

	// Raise the first line
	top += verticalOffset * 2;

	// Iterators for the caption
	Ogre::DisplayString::iterator i, iend;

	// Control if caption is wider than the text area width
	// the returned caption will have \n added where the line needs to break in order to fit in the box
	ForcedLineBreaks forcedLineBreaks;
	calculateLineBreaks(forcedLineBreaks);

	//Raise the first line of the caption (depending on the vertical alignment)
	if ( mVerticalAlignment != TOP )
	{
		iend = mCaption.end();
		for (i = mCaption.begin(); i != iend; ++i)
		{
			size_t index = i - mCaption.begin();
			// TODO: optimized this
			bool isLineBreak = std::find(forcedLineBreaks.begin(), forcedLineBreaks.end(), index)
					!= forcedLineBreaks.end();

			if ( IsNewLine(*i) || isLineBreak )
				top += verticalOffset * 2.0;
		}
	}

	// Calculate the max number of lines depending on the size of the text area
	int maxLines = -1;
	if ( mCharHeight > 0 )
		maxLines = floor(m_textAreaHeight / mCharHeight);
	int nLines = 0;

	// iterate through the caption to render the characters
	bool first = true;
	bool newLine = true;
	float lineLength = 0;
	iend = mCaption.end();
	for (i = mCaption.begin(); i != iend; ++i)
	{
		// number of lines counter (to ensure the text does not go beyond text area limits)
		if ( newLine )
			nLines++;

		// Check vertical limit
		if ( (maxLines > 0) && (nLines > maxLines) )
		{
			// Just consume the character (reduce vertext count)
			mRenderOp.vertexData->vertexCount -= 6;
			continue;
		}

		// If we are starting a new line -> calculate the offsets depending on the next characters and current alignment
		if ( newLine )
		{
			// Calculate new line left coordinate (start of the line -> depends on horizontal alignment)
			lineLength = 0;
			calculateLineLeft(i, iend, forcedLineBreaks, left, lineLength);

			// now continue with the rest of the line
			newLine = false;
		}

		// Deal with the current character
		Ogre::Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(i);

		// Check if this should be a line break due to the text area width (that is smaller than the text)
		size_t index = i - mCaption.begin();
		bool isLineBreak = std::find(forcedLineBreaks.begin(), forcedLineBreaks.end(), index) != forcedLineBreaks.end();

		// Current character is a new line
		if ( IsNewLine(character) )
		{
			left = 0 * 2.0 - 1.0;
			top -= mCharHeight * 2.0;
			newLine = true;
			// Also reduce tri count
			mRenderOp.vertexData->vertexCount -= 6;

			// consume CR/LF in one
			if ( character == UNICODE_CR )
			{
				Ogre::DisplayString::iterator peeki = i;
				peeki++;
				if ( peeki != iend && OGRE_DEREF_DISPLAYSTRING_ITERATOR(peeki) == UNICODE_LF )
				{
					i = peeki; // skip both as one newline
					// Also reduce tri count
					mRenderOp.vertexData->vertexCount -= 6;
				}

			}
			continue;
		}

		// If this is a forced line break (calculated to make the text fit the text area width
		else if ( isLineBreak )
		{
			// Normal line break stuff (but without reducing tris)
			left = 0 * 2.0 - 1.0;
			top -= mCharHeight * 2.0;
			newLine = true;

			// Remove this line break from the list of line breaks to process
			forcedLineBreaks.remove(index);

			// Go back one index, so the ++i of the for loop don't skip the current actual character
			// (as the line break has been forced by us to make the text fit)
			if ( IsSpace(character) )
				mRenderOp.vertexData->vertexCount -= 6;
			else
				--i;
			continue;
		} else if ( IsSpace(character) ) // space

		{
			// Just leave a gap, no tris
			left += mSpaceWidth;
			// Also reduce tri count
			mRenderOp.vertexData->vertexCount -= 6;
			continue;
		}

		// This is a normal character -> setup its geometry
		drawCharacter(pVert, character, top, left, lineLength);
	}
	// Unlock vertex buffer
	vbuf->unlock();

	// Update color?
	//if (mUpdateColors)
	//	this->_updateColors();

	// update AABB/Sphere radius (for ogre movable object)
	// Note: for some strange reason texts must be scale by 2 to have the correct pixel size
	mAABB = AABox(m_min/2, m_max/2);
	mRadius = Ogre::Math::Sqrt(m_maxSquaredRadius / 2);

	// update done
	mNeedUpdate = false;
}

void MovableText::_updateColors( void )
{
	assert(!mpFont.isNull());
	assert(!mpMaterial.isNull());

	// Convert to system-specific
	Ogre::RGBA color;
	Ogre::Root::getSingleton().convertColourValue(mColor, &color);
	Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);
	Ogre::RGBA *pDest = static_cast<Ogre::RGBA*> (vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	for (int i = 0; i < (int) mRenderOp.vertexData->vertexCount; ++i)
		*pDest++ = color;
	vbuf->unlock();
	mUpdateColors = false;
}

const Quaternion& MovableText::getWorldOrientation( void ) const
{
	assert(mpCam);
	return const_cast<Quaternion&> (mpCam->getDerivedOrientation());
}

// Add to build on Shoggoth:
void MovableText::visitRenderables( Ogre::Renderable::Visitor* visitor, bool debugRenderables )
{
}

const Vector& MovableText::getWorldPosition( void ) const
{
	assert(mParentNode);
	return mParentNode->_getDerivedPosition();
}

void MovableText::getWorldTransforms( Matrix4 *xform ) const
{
	// Visibility test
	if ( !this->isVisible() )
		return;

	// Billboard way
	if ( mAlwaysFaceCamera && mpCam )
	{
		Matrix3 rot3x3, scale3x3 = Matrix3::IDENTITY;

		// store rotation in a matrix
		mpCam->getDerivedOrientation().ToRotationMatrix(rot3x3);

		// apply scale
		scale3x3[0][0] = mParentNode->_getDerivedScale().x / 2;
		scale3x3[1][1] = mParentNode->_getDerivedScale().y / 2;
		scale3x3[2][2] = mParentNode->_getDerivedScale().z / 2;

		// apply all transforms to xform
		*xform = (rot3x3 * scale3x3);
		xform->setTrans(mParentNode->_getDerivedPosition());
	}
	// 3d -> rotations allowed

	else
	{
		// apply scale from parent (divided by 2... TODO: not sure why is necessary)
		Matrix3 rot3x3, scale3x3 = Matrix3::IDENTITY;
		scale3x3[0][0] = mParentNode->_getDerivedScale().x / 2;
		scale3x3[1][1] = mParentNode->_getDerivedScale().y / 2;
		scale3x3[2][2] = mParentNode->_getDerivedScale().z / 2;

		// parent node rotation
		mParentNode->_getDerivedOrientation().ToRotationMatrix(rot3x3);

		// TEMP hasta que se ajuste sistema de coordenadas global!!!
		//mpCam->getDerivedOrientation().ToRotationMatrix(rot3x3);

		// apply all transforms (scale + translation)to xform
		*xform = (rot3x3 * scale3x3);
		xform->setTrans(mParentNode->_getDerivedPosition());

		// FIRST TIME TRIED (Working)
		//*xform = mParentNode->_getFullTransform();
	}
}

void MovableText::getRenderOperation( Ogre::RenderOperation &op )
{
	if ( this->isVisible() )
	{
		if ( mNeedUpdate )
			this->_setupGeometry();
		if ( mUpdateColors )
			this->_updateColors();
		op = mRenderOp;
	}
}

void MovableText::_notifyCurrentCamera( Ogre::Camera *cam )
{
	mpCam = cam;
}

void MovableText::_updateRenderQueue( Ogre::RenderQueue* queue )
{
	if ( this->isVisible() )
	{
		if ( mNeedUpdate )
			this->_setupGeometry();
		if ( mUpdateColors )
			this->_updateColors();

		queue->addRenderable(this, mRenderQueueID, OGRE_RENDERABLE_DEFAULT_PRIORITY);
		//queue->addRenderable(this, mRenderQueueID, RENDER_QUEUE_SKIES_LATE);
	}
}

void MovableText::calculateLineBreaks( ForcedLineBreaks& forcedLineBreaks )
{
	// TODO: Optimize this method

	// Need to break any line due to size limit?
	if ( m_textAreaWidth > 0 )
	{
		float lineLength = 0;
		bool breakToWrapWord = false;
		for (Ogre::DisplayString::iterator i = mCaption.begin(); i != mCaption.end(); ++i)
		{
			// Get current character
			Ogre::Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(i);

			// New line?
			if ( IsNewLine(character) )
			{
				lineLength = 0;
				continue;
			}
			// Space?

			else if ( IsSpace(character) )
			{
				lineLength += mSpaceWidth;
			}
			// "Normal" character

			else
			{
				// Get the length of the next word
				float nextWordLength = getWordLength(i, mCaption.end());

				// Wordwrap is activated -> check whole words (only if possible, maybe the word itself is longer than textarea width)
				if ( mwordWrap && (nextWordLength < m_textAreaWidth))
				{
					// have to break the line?
					if ( (lineLength + nextWordLength) > m_textAreaWidth )
						breakToWrapWord = true;
					// word fits altogether

					else
					{
						lineLength += nextWordLength;
						Ogre::DisplayString::iterator captionEndIt = mCaption.end();
						advanceWord(i, captionEndIt);
					}
				}
				// Check just one character at a time

				else
					lineLength += mpFont->getGlyphAspectRatio(character) * mCharHeight; // * 2.0 * mViewportAspectCoef;
			}

			// If the text does not fit in the width anymore -> insert line break
			if ( (lineLength > m_textAreaWidth) || breakToWrapWord )
			{
				// Insert forced line break to fit in the text area space
				int lineBreakIndex = (i - mCaption.begin()) + 1;

				// If the break is due to the word wrap we need to go back one index (so that the word is not split)
				if ( breakToWrapWord )
					lineBreakIndex--;

				// Store the line break for later processing
				forcedLineBreaks.push_back(lineBreakIndex);

				// Reset vars
				lineLength = 0;
				breakToWrapWord = false;
			}

		}
	}
}

float MovableText::getWordLength( Ogre::DisplayString::iterator it, const Ogre::DisplayString::iterator& end )
{
	float length = 0;
	while ( (it != end) && !IsNewLine(*it) && !IsSpace(*it) )
	{
		length += mpFont->getGlyphAspectRatio(*it) * mCharHeight;
		++it;
	}

	return length;
}

void MovableText::advanceWord( Ogre::DisplayString::iterator& it, const Ogre::DisplayString::iterator& end )
{
	while ( ((it + 1) != end) && !IsNewLine(*(it + 1)) && !IsSpace(*(it + 1)))
		++it;
}

void MovableText::calculateLineLeft( const Ogre::DisplayString::iterator& currentIt,
		const Ogre::DisplayString::iterator& iend, const ForcedLineBreaks& forcedLineBreaks, float& left,
		float& lineLength )
{
	// This is only necessary if alignment is not left (because in that case the line starts at the node coordinates)
	if ( mHorizontalAlignment != LEFT )
	{
		for (Ogre::DisplayString::iterator j = currentIt; j != iend; j++)
		{
			Ogre::Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(j);
			size_t index = j - mCaption.begin();
			if ( IsNewLine(character) || (std::find(forcedLineBreaks.begin(), forcedLineBreaks.end(), index)
					!= forcedLineBreaks.end()) )
			{
				break;
			} else if ( IsSpace(character) ) // space

			{
				lineLength += mSpaceWidth;
			} else
			{
				lineLength += mpFont->getGlyphAspectRatio(character) * mCharHeight * 2.0 * mViewportAspectCoef;
			}
		}

		// Manage alignment offset (if it is not left)
		if ( mHorizontalAlignment == RIGHT )
			left -= lineLength;
		else if ( mHorizontalAlignment == CENTER )
			left -= lineLength * 0.5;
	}
}

void MovableText::drawCharacter( float*& pVert, const Ogre::Font::CodePoint& character, float& top, float& left,
		float& lineLength )
{
	Vector currPos;
	float horiz_height = mpFont->getGlyphAspectRatio(character) * mViewportAspectCoef;
	const Ogre::Font::UVRect& uvRect = mpFont->getGlyphTexCoords(character);

	// each vert is (x, y, z, u, v)
	//-------------------------------------------------------------------------------------
	// First tri
	//
	// Upper left
	*pVert++ = left;
	*pVert++ = top;
	*pVert++ = -1.0;
	*pVert++ = uvRect.left;
	*pVert++ = uvRect.top;

	// Deal with bounds (for Ogre Movable Object BBox and Radius)
	if ( mHorizontalAlignment == LEFT )
		currPos = Vector(0, top, -1.0);
	else if ( mHorizontalAlignment == CENTER )
		currPos = Vector((lineLength / 2), top, -1.0);
	else
		currPos = Vector(-lineLength, top, -1.0);

	// First triangle -> init the vars to calculate the bbox
	if ( m_maxSquaredRadius < 0 )
	{
		m_min = m_max = currPos;
		m_maxSquaredRadius = currPos.squaredLength();
	}
	// Not the first tri -> update the vars to calculate the bbox

	else
	{
		m_min.makeFloor(currPos);
		m_max.makeCeil(currPos);
		m_maxSquaredRadius = std::max(m_maxSquaredRadius, currPos.squaredLength());
	}

	top -= mCharHeight * 2.0;

	// Bottom left
	*pVert++ = left;
	*pVert++ = top;
	*pVert++ = -1.0;
	*pVert++ = uvRect.left;
	*pVert++ = uvRect.bottom;

	// Deal with bounds (for Ogre Movable Object BBox and Radius)
	if ( mHorizontalAlignment == LEFT )
		currPos = Vector(0, top, -1.0);
	else if ( mHorizontalAlignment == CENTER )
		currPos = Vector((lineLength / 2), top, -1.0);
	else
		currPos = Vector(-lineLength, top, -1.0);

	m_min.makeFloor(currPos);
	m_max.makeCeil(currPos);
	m_maxSquaredRadius = std::max(m_maxSquaredRadius, currPos.squaredLength());

	top += mCharHeight * 2.0;
	left += horiz_height * mCharHeight * 2.0;

	// Top right
	*pVert++ = left;
	*pVert++ = top;
	*pVert++ = -1.0;
	*pVert++ = uvRect.right;
	*pVert++ = uvRect.top;

	// Deal with bounds
	if ( mHorizontalAlignment == LEFT )
		currPos = Vector(0, top, -1.0);
	else if ( mHorizontalAlignment == CENTER )
		currPos = Vector((lineLength / 2), top, -1.0);
	else
		currPos = Vector(-lineLength, top, -1.0);

	m_min.makeFloor(currPos);
	m_max.makeCeil(currPos);
	m_maxSquaredRadius = std::max(m_maxSquaredRadius, currPos.squaredLength());

	//-------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------
	// Second tri
	//
	// Top right (again)
	*pVert++ = left;
	*pVert++ = top;
	*pVert++ = -1.0;
	*pVert++ = uvRect.right;
	*pVert++ = uvRect.top;

	// Deal with bounds
	currPos = Vector(left, top, -1.0);
	m_min.makeFloor(currPos);
	m_max.makeCeil(currPos);
	m_maxSquaredRadius = std::max(m_maxSquaredRadius, currPos.squaredLength());

	top -= mCharHeight * 2.0;
	left -= horiz_height * mCharHeight * 2.0;

	// Bottom left (again)
	*pVert++ = left;
	*pVert++ = top;
	*pVert++ = -1.0;
	*pVert++ = uvRect.left;
	*pVert++ = uvRect.bottom;

	// Deal with bounds
	currPos = Vector(left, top, -1.0);
	m_min.makeFloor(currPos);
	m_max.makeCeil(currPos);
	m_maxSquaredRadius = std::max(m_maxSquaredRadius, currPos.squaredLength());

	left += horiz_height * mCharHeight * 2.0;

	// Bottom right
	*pVert++ = left;
	*pVert++ = top;
	*pVert++ = -1.0;
	*pVert++ = uvRect.right;
	*pVert++ = uvRect.bottom;

	// Deal with bounds
	currPos = Vector(left, top, -1.0);
	m_min.makeFloor(currPos);
	m_max.makeCeil(currPos);
	m_maxSquaredRadius = std::max(m_maxSquaredRadius, currPos.squaredLength());

	//-------------------------------------------------------------------------------------

	// Go back up with top
	top += mCharHeight * 2.0;
}


void MovableText::set2dRendering()
{
	if ( mRender2D == false )
	{
		// Set render properties
		setUseIdentityProjection(true);
		setUseIdentityView(true);
		setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY -1);

		// Adjust scale
		float scaleFactor = (mCharHeight / (float)height) / 2.0f;

		mScale.x *= scaleFactor;
		mScale.y *= scaleFactor;
		mScale.z = 1;

		setScale( mScale.x, mScale.y );
		
		mRender2D = true;
	}

	// Note: This is outside the if, becuase it is overriden every frame... (fix this!)
	mAABB.setInfinite();
}

void MovableText::set3dRendering()
{
	// set 3d rendering
	if ( mRender2D == true )
	{
		// Set render properties
		setUseIdentityProjection(false);
		setUseIdentityView(false);

		// Adjust scale
		float scaleFactor = (mCharHeight / (float)height);

		mScale.x /= scaleFactor;
		mScale.y /= scaleFactor;
		mScale.z = 1;

		setScale( mScale );

		mRender2D = false;
	}
	
}

}
