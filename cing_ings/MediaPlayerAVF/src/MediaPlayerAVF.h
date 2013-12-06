//
//  MediaPlayerAVF.h
//  Cing
//
//  Created by Julio Obelleiro on 8/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#ifndef __Cing__MediaPlayerAVF__
#define __Cing__MediaPlayerAVF__

#include <iostream>

// Graphics
#include "graphics/Image.h"

// Ogre
#include "OgreTimer.h"

#include "AVFVideoWrapper.h"

namespace Cing
{
    

class MediaPlayerAVF
{
public:
    
    // Constructor/Destructoc
    MediaPlayerAVF();
    ~MediaPlayerAVF();
    
    // Lifecycle
    bool            init    ();
    //NOTE: AVFoundation player seems to only work (return frames) when format is set to BGRA)
    bool            load    ( const std::string& fileName, GraphicsType requestedVideoFormat = BGRA, float fps = -1 );
    void            end     ();
    void            update  ( bool updateTexture = false );
    Image&          getImage();
    
    // Query
    bool            isValid     () const { return m_bIsValid; }
    bool            isPlaying   ();
    bool            isPaused    ();
    float           duration    () const { return (float)m_videoDuration; }
    float           time        ();
    int             getWidth    () const { return m_videoWidth; }
    int             getHeight   () const { return m_videoHeight; }
    float           fps         () const { return m_videoFps; }
    unsigned int    frameCount  () const { return m_videoNFrames; }
    unsigned int    currentFrameNumber();
    bool            hasNewFrame () const { return m_newBufferReady; }
    
    
    // Media control
    void            play    ();
    void            loop    ();
    void            stop    ();
    void            pause   ();
    void            setLoop ( bool loop ) { m_loop = loop; }
    void            noLoop  () { setLoop( false  ); }
    void            jump    ( float whereInSecs );
    void            jumpToFrame ( unsigned int frameNumber );
    void            nextFrame   ();
    void            speed   ( float rate );
    
private:
    // Private methods
    bool            buildPathToFile( const std::string& filePath );
    AVFPixelFormat  toAVFPixelFormat( GraphicsType cingPixelFormat );
    
    // AVFoundation attributes
    AVFVideoWrapper*       m_player;            ///< This is the AVF player wrapper (access to AVF)
    
    
    // Media info
    std::string				m_fileName;			///< Name of the loaded video
    std::string				m_filePath;			///< Full path of the loaded video
    double         			m_videoDuration;	///< Duration in seconds
    float                 	m_videoFps;			///< Frames per second of the loaded video
    float                   m_desiredFps;       ///< Fps set by the user (might not be doable).
    int						m_videoNFrames;		///< Total Number of frames
    int                   	m_videoWidth;		///< Width in pixels of the loaded video
    int                   	m_videoHeight;		///< Height in pixels of the loaded video
    
    // Playback Settings
    bool                  	m_loop;				///< If true, the video will loop
    bool					m_loopPending;		///< Loop pending
    
    // Audio settings
    bool					m_mute;				///< True if volume is mutted (0)
    float					m_volume;			///< Current audio volume (0..1)
    
    // Playback control
    Ogre::Timer				m_timer;			///< Used to control de playback
    int						m_currentFrame;		///< Current frame (last frame retrieved from capture)
    bool					m_playing;			///< True when the player is active (playing)
    bool                    m_paused;           ///< True when the player has been paused (when play or loop is called next, playhead will resume from the position where it was paused).
    
    // Buffer Stuff
    int						m_bufferSizeInBytes;///< Size of the buffer in bytes (w*h*nChannels)
    GraphicsType            m_pixelFormat;		///< Pixel format in which new image frames will be stored
    Image                   m_frameImg;			///< Image containing the buffer of the current video frame
    bool					m_newBufferReady;	///< True when there is a new buffer ready
    
    // Internal stuff
    bool                  	m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
    
};
    
}
#endif /* defined(__Cing__MediaPlayerAVF__) */
