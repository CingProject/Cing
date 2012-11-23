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

#ifndef __Cing__RTTRectSaveManager__H__
#define __Cing__RTTRectSaveManager__H__

// Precompiled headers
#include "Cing-Precompiled.h"

#include <string>
#include "common/Singleton.h"
#include "graphics/Image.h"
#include "PTypes/include/pasync.h"

namespace Cing
{
// Class to save rectangles / areas taken from an image, in a separate thread.
class RTTRectSaveManager
    : public Singleton<RTTRectSaveManager>
{
public:
    RTTRectSaveManager();
    ~RTTRectSaveManager();

    void storePicture( const std::string& fileName, const Rect& rect, Ogre::PixelBox* picture );

private:

    class PictureWriter: public pt::thread
    {
    public:
        PictureWriter( const std::string& fileName, const Rect& rect, Ogre::PixelBox* picture );
        ~PictureWriter();

        virtual void execute();
        virtual void cleanup();

    private:
        std::string		fileName;
        Ogre::PixelBox* picture;
        Rect			rect;
    };

};

}

#endif