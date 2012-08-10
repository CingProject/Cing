/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

----------------------------------------------------------------------------------------------------------
 File: RTTRectSaveManager.cpp
 Desc: Class to store a rectangle taken from the current frame

 @note: the class takes ownership of the image and deallocates it when the data has been written.
 ----------------------------------------------------------------------------------------------------------
*/


// Precompiled headers
#include "Cing-Precompiled.h"

#include "RTTRectSaveManager.h"
#include "Graphics/Color.h"
#include "Common/CommonTypes.h"

namespace Cing
{

RTTRectSaveManager::RTTRectSaveManager()
{
}

RTTRectSaveManager::~RTTRectSaveManager()
{
}

void RTTRectSaveManager::storePicture( const std::string& fileName, const Rect& rect, Ogre::PixelBox* picture )
{
    RTTRectSaveManager::PictureWriter* writer = new RTTRectSaveManager::PictureWriter( fileName, rect, picture );
    writer->start();
}

RTTRectSaveManager::PictureWriter::PictureWriter( const std::string& aFileName, const Rect& _rect, Ogre::PixelBox* aPicture )
    : pt::thread( true )
    , fileName( aFileName )
	, rect( _rect )
    , picture( aPicture )
{
}

RTTRectSaveManager::PictureWriter::~PictureWriter()
{
    cleanup();
}

void RTTRectSaveManager::PictureWriter::execute()
{
	BoxArea box((size_t)rect.left, (size_t)rect.top, (size_t)rect.right, (size_t)rect.bottom);
    Ogre::PixelBox pbox = picture->getSubVolume( box );

    size_t elementSize = Ogre::PixelUtil::getMemorySize( 1, 1, 1, picture->format );
    size_t w = pbox.getWidth();
    size_t h = pbox.getHeight();
    void* data = malloc( elementSize*w*h );
    unsigned char* wptr = (unsigned char*)data;
    unsigned char* rptr = (unsigned char*)pbox.data;

    for ( size_t j = 0; j < h; j++ )
    {
        memcpy( wptr, rptr, elementSize*w );
        wptr += elementSize*w;
        rptr += elementSize*(w + pbox.getRowSkip());
    }

    Ogre::Image pic;
    pic.loadDynamicImage( (Ogre::uchar*)data, w, h, 1, picture->format );
    pic.save( fileName );

    free( data );
}

void RTTRectSaveManager::PictureWriter::cleanup()
{
    if ( picture )
    {
        if ( picture->data )
        {
            free( picture->data );
            picture->data = NULL;
        }

        delete picture;
        picture = NULL;
    }
}
} // namespace