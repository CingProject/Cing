// Precompiled headers
#include "Cing-Precompiled.h"

// Header
#include "BaseVideoRecorder.h"

// Cing
#include "common/LogManager.h"
#include "common/Exception.h"
#include "common/SystemUtils.h"
#include "framework/UserAppGlobals.h"

namespace Cing
{

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	BaseVideoRecorder::BaseVideoRecorder():
m_width             		( 0     ), 
m_height            		( 0     ), 
m_fps               		( 25     ), 
m_bIsValid          		( false )
{
}

/**
* @internal
* @brief Destructor. Class release.
*/
BaseVideoRecorder::~BaseVideoRecorder()
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
* @param[in] fps		frames per second to capture
*/
bool BaseVideoRecorder::init( int width, int height, const std::string& fileName, float fps /*= 25*/)
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Build absolute path
	m_path = fileName;
	if ( isPathAbsolute( m_path ) == false )
	{	
		m_path = dataFolder + m_path;
	}

	if( m_path.size() == 0 )
		LOG_CRITICAL("OCVVideoRecorder::init() - Output file name inside VideoRecorder is empty, you must provide a name for the output file");

	// Store data
	m_width			= width;
	m_height		= height;
	m_fps			= fps;

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
* @internal
* @brief Releases the class resources. 
* After this method is called the class is not valid anymore.
*/
void BaseVideoRecorder::end()
{
	// Check if the class is already released
	if ( !isValid() )
		return;

	// The class is not valid anymore
	m_bIsValid = false;
}


} // namespace