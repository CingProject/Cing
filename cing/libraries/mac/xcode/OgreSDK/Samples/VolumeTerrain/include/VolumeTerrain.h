/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

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
-----------------------------------------------------------------------------
*/
#ifndef __VolumeTerrain_H__
#define __VolumeTerrain_H__

#include <stdio.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <fcntl.h>
#include <io.h>
#endif
#include <iostream>
#include <string>

#include "SdkSample.h"

#include "OgreVolumeChunk.h"

using namespace Ogre;
using namespace OgreBites;
using namespace Ogre::Volume;

/** Sample for the volume terrain.
*/
class _OgreSampleClassExport Sample_VolumeTerrain : public SdkSample
{
private:
    
    /// Holds the volume root.
    Chunk *mVolumeRoot;

    /// Holds the read volume parameters.
    ChunkParameters mParameters;

    /// The node on which the terrain is attached.
    SceneNode *mVolumeRootNode;

protected:
    
    /// To show or hide everything.
    bool mHideAll;

    /** Sets up the sample.
    */
    virtual void setupContent(void);
        
    /** Sets up the UI.
    */
    virtual void setupControls(void);
        
    /** Is called when the sample is stopped.
    */
    virtual void cleanupContent(void);

    /** Intersects a ray with the volume and adds a sphere at the intersection.
    @param ray
        The ray.
    @param doUnion
        Whether to add or subtract a sphere
    */
    void shootRay(Ray ray, bool doUnion);
public:

    /** Constructor.
    */
    Sample_VolumeTerrain(void);
    
    /** Overridden from SdkSample.
    */
    virtual bool keyPressed(const OIS::KeyEvent& evt);
    
    /** Overridden from SdkSample.
    */
#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS) || (OGRE_PLATFORM == OGRE_PLATFORM_ANDROID)
    virtual bool touchPressed(const OIS::MultiTouchEvent& evt);
#else
    virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
#endif
};

#endif
