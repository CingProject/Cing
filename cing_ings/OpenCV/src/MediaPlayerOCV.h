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

#ifndef _MediaPlayerOCV_H_
#define _MediaPlayerOCV_H_

// Precompiled headers
#include "Cing-Precompiled.h"


// Graphics
#include "graphics/Image.h"

// Ogre
#include "OgreTimer.h"

// OpenCV
#include "opencv2/highgui/highgui.hpp"

// Threading related
#include <boost/thread/mutex.hpp>
// Extern
#undef nil
#include "PTypes/include/pasync.h"

namespace Cing
{
	/**
	* Media player that uses GStreamer to decode and play video/audio
	*/
	class MediaPlayerOCV
	{
	public:
        
        // structure to capture the image from the camera in a different thread
        struct OCVCaptureThread: public pt::thread
        {
            OCVCaptureThread( MediaPlayerOCV& player, Ogre::Timer& timer, float fps ): pt::thread( false ), m_player( player ), m_timer(timer), m_fps(fps) {};
            void execute();
            void cleanup();
            
            cv::Mat             m_localMat;
            MediaPlayerOCV&     m_player;
            Ogre::Timer&        m_timer;
            float               m_fps;
        };
        

		// Constructor / Destructor
		MediaPlayerOCV();
		MediaPlayerOCV( const std::string& filename, float fps = -1 );
		~MediaPlayerOCV();

		// Init / Release / Update
		bool    init    ();
		bool    load    ( const std::string& fileName, GraphicsType requestedVideoFormat = RGB, float fps = -1 );
		void    end     ();
		void    update  ( bool updateTexture = false );
		Image&  getImage();

        // General player settings
        void    setMultithreaded ( bool threaded ) { m_multithreaded = threaded; } // NOTE: it has to be called before load.
        
		// Query methods
		bool    isValid     () const { return m_bIsValid; }
		bool    isPlaying   ();
		float   duration    () const { return (float)m_videoDuration; }
		float   time        ();
		int     getWidth    () const { return m_videoWidth; }
		int     getHeight   () const { return m_videoHeight; }
        float   fps         () const { return m_videoFps; }

		// Media control
		void    play    ();
		void    loop    ();
		void    stop    ();
		void    pause   ();
		void    setLoop ( bool loop ) { m_loop = loop; }
		void    noLoop  () { setLoop( false  ); }
		void    jump    ( float whereInSecs );
		void    speed   ( float rate );

		// Audio Specific Control
		void	toggleMute();
		void	setVolume ( float volume );
		float	getVolume () const;

	private:
		// Internal methods
		bool			buildPathToFile			( const std::string& path );
		void			copyBufferIntoImage		( bool updateTexture = false ); // from update in single thread mode
        
        // Threaded version
        void            setNewFrame             ( unsigned int currentFrameNumber, cv::Mat& currentFrame ); // from thread in multithreaded mode
        void            copyFrameFromThread     ();
        
		// Open CV stuff
		cv::VideoCapture		m_capture;          ///< OpenCV capture to read movie file
        OCVCaptureThread        *m_captureThread;   ///< Thread to capture images from the video
        cv::Mat                 m_bufferFromThread; ///< Intermediate buffer to read/write from thread

		// Media info
		String					m_fileName;			///< Name of the loaded video
		String					m_filePath;			///< Full path of the loaded video
		double         			m_videoDuration;	///< Duration in seconds
		float                 	m_videoFps;			///< Frames per second of the loaded video
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
		
		// Buffer Stuff
        bool                    m_multithreaded;    ///< If true, the frame capture from the video happens in a separate thread.
        boost::try_mutex        m_mutex;            ///< Mutex used to sync read/writes from capture thread and main thread
		int						m_bufferSizeInBytes;///< Size of the buffer in bytes (w*h*nChannels)
		GraphicsType			m_pixelFormat;		///< Pixel format in which new image frames will be stored
		Image					m_frameImg;			///< Image containing the buffer of the current video frame
		bool					m_newBufferReady;	///< True when there is a new buffer ready

		// Internal stuff
		bool                  	m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};

} // namespace

#endif // _MediaPlayerOCV_H_
