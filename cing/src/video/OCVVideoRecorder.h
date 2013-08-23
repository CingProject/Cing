#ifndef _OCVVideoRecorder_H_
#define _OCVVideoRecorder_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "BaseVideoRecorder.h"

// OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace Cing
{

	/**
	* @internal
	* Class to record a video file based on OpenCV library
	*/
	class OCVVideoRecorder: public BaseVideoRecorder
	{
	public:

		// Constructor / Destructor
		OCVVideoRecorder();
		virtual ~OCVVideoRecorder();

		// BaseVideoRecorder Interface
		virtual bool		init( int width, int height, const std::string& fileName, float fps = 25, int fourcc = CV_FOURCC('D', 'I', 'B', ' ')  );
		virtual void		end();
		virtual void		update();

		// Video writing control
		virtual void		addFrame( Cing::Image& newFrame );
		virtual void		finish();

		// Query methods
		bool				isValid() const { return m_bIsValid; }

	private:

		cv::VideoWriter*	m_cvVideoWriter;	///< OpenCV class to record video to disk
		bool				m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};

} // namespace

#endif // _OCVVideoRecorder_H_
