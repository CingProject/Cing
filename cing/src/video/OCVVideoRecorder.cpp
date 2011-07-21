// Precompiled headers
#include "Cing-Precompiled.h"

#include "OCVVideoRecorder.h"

// OpenCv
#include "OpenCV/cv.h"
#include "OpenCV/cxcore.h"

// Common
#include "common/Exception.h"
#include "common/Release.h"
#include "common/LogManager.h"

namespace Cing
{

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	OCVVideoRecorder::OCVVideoRecorder():
m_bIsValid        ( false )
{
}

/**
* @internal
* @brief Destructor. Class release.
*/
OCVVideoRecorder::~OCVVideoRecorder()
{
	// Release resources
	end();
}

/**
* @internal
* @brief Initializes the class so it becomes valid.
*
* @param[in] width		Width resolution
* @param[in] height		Height resolution
* @param[in] fileName	Output file name
* @param[in] fps		FPS
* @param[in] fourcc		Defines the codec to be used  (it should be on the system to work). Default is DIB which is uncompressed avi
* @return true if the initialization was ok | false otherwise
*/
bool OCVVideoRecorder::init( int width, int height , const std::string& fileName, float fps /* = 25 */, int fourcc /*= CV_FOURCC('D', 'I', 'B', ' ')*/ )
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Init base class (with actual capture resolution)
	BaseVideoRecorder::init( width, height, fileName, fps );

	LOG( "OCVVideoRecorder::init. Creating video file at location %s", m_path.c_str() );

	// TODO: Check CV_FOURCC manual initialization failure
	try
	{
		m_cvVideoWriter = new cv::VideoWriter( m_path.c_str(), fourcc, fps, cvSize( m_width, m_height), true );
	}
	catch( cv::Exception& e )
	{
		LOG_ERROR( "OCVVideoRecorder::init. Error creating video file: %s", e.what() );
		return false;
	}

	if ( m_cvVideoWriter->isOpened() )
	{
		LOG( "Video recorder correctly initialized" );
		m_bIsValid = true;	// The class is now initialized
	}
	else
	{
		LOG_ERROR( "OCVVideoRecorder::init Error: Video recorder could not be initialized. Maybe the FourCC/Codec is not available in the system?" );
		m_bIsValid = false;
	}

	return m_bIsValid;
}

/**
* @internal
* @brief Releases the class resources. After this method is called the class is not valid anymore.
*/
void OCVVideoRecorder::end()
{
	BaseVideoRecorder::end();

	// Check if the class is already released
	if ( !isValid() )
		return;

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
* @internal
*/
void OCVVideoRecorder::update()
{
}

/**
* @public
* @brief Add a new frame
*/
void OCVVideoRecorder::addFrame( Cing::Image& newFrame )
{
	bool isOpen = m_cvVideoWriter->isOpened();

	if (isOpen)
	{
		// HACK: Convert RGB2BGR then BGR2RGB
		//cvtColor(newFrame.getCVMat(), newFrame.getCVMat(), CV_RGB2BGR);
		(*m_cvVideoWriter) << newFrame.getCVMat();
		//cvtColor(newFrame.getCVMat(), newFrame.getCVMat(), CV_BGR2RGB);
	}
	else
		LOG_CRITICAL("OCVVideoRecorder::addFrame() - Warning: OCVVideoRecorder is closed");
}

/**
* @public
* @brief Close the file and writes to disk
*/
void OCVVideoRecorder::finish()
{
	// File is written to disk when calling the destructor
	delete m_cvVideoWriter;
	m_cvVideoWriter = NULL;

	end();

	/*
	// TODO: Check this
	// Create a new cv::VideoWriter to allow future use of this object
	m_cvVideoWriter = new cv::VideoWriter( m_fileName.c_str(), -1, m_fps, cvSize( m_width, m_height), true );
	*/
}


} // namespace