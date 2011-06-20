// Precompiled headers
#include "Cing-Precompiled.h"

#include "OCVVideoRecorder.h"

// OpenCv
#include "OpenCV/cv.h"

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
* @param[in] width		width resolution capture
* @param[in] height		height resolution capture
* @param[in] fileName	Output file name
*/
bool OCVVideoRecorder::init( int width, int height ,std::string fileName)
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Init base class (with actual capture resolution)
	BaseVideoRecorder::init(  width, height,fileName );

	// Init OpenCv video recorder. Open a dialog box for CODEC selection...
	//m_cvVideoWriter = new cv::VideoWriter( m_fileName.c_str(), -1, m_fps, cvSize( m_width, m_height), true );

	// TODO: Check CV_FOURCC manual initialization failure
	m_cvVideoWriter = new cv::VideoWriter( m_fileName.c_str(), CV_FOURCC('D', 'I', 'B', ' '), m_fps, cvSize( m_width, m_height), true );

	if ( m_cvVideoWriter->isOpened() )
		m_bIsValid = true;	// The class is now initialized
	else
		m_bIsValid = false;

	return m_bIsValid;
}

/**
* @internal
* @brief Initializes the class so it becomes valid.
*
* @param[in] width		Width resolution
* @param[in] height		Height resolution
* @param[in] fileName	Output file name
* @param[in] fps		FPS
* @return true if the initialization was ok | false otherwise
*/
bool OCVVideoRecorder::init( int width, int height ,std::string fileName, int fps/* = 25 */)
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Init base class (with actual capture resolution)
	BaseVideoRecorder::init( width, height, fileName, fps );

	// Init OpenCv video recorder. Open a dialog box for CODEC selection...
	//m_cvVideoWriter = new cv::VideoWriter( m_fileName.c_str(), -1, m_fps, cvSize( m_width, m_height), true );

	// TODO: Check CV_FOURCC manual initialization failure
	m_cvVideoWriter = new cv::VideoWriter( m_fileName.c_str(), CV_FOURCC('D', 'I', 'B', ' '), m_fps, cvSize( m_width, m_height), true );

	if ( m_cvVideoWriter->isOpened() )
		m_bIsValid = true;	// The class is now initialized
	else
		m_bIsValid = false;

	return m_bIsValid;
}

/**
* @internal
* @brief Releases the class resources. After this method is called the class is not valid anymore.
*/
void OCVVideoRecorder::end()
{
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
		cvtColor(newFrame.getCVMat(), newFrame.getCVMat(), CV_RGB2BGR);
		(*m_cvVideoWriter) << newFrame.getCVMat();
		cvtColor(newFrame.getCVMat(), newFrame.getCVMat(), CV_BGR2RGB);
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