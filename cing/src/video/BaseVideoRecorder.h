#ifndef _BaseVideoRecorder_H_
#define _BaseVideoRecorder_H_

// Precompiled headers
#include "Cing-Precompiled.h"

// Cing
#include "graphics\GraphicsTypes.h"
#include "graphics\Image.h"

namespace Cing
{

	/**
	* @internal
	* Base class for all the video recorder implementations.
	*/
	class BaseVideoRecorder
	{
	public:

		// Constructor / Destructor
		BaseVideoRecorder ();
		virtual ~BaseVideoRecorder();

		// Init / Release / Update (to be implemented in subclasses)
		virtual bool		init            ( int width, int height, std::string fileName);
		virtual bool		init            ( int width, int height, std::string fileName, float fps);

		//virtual bool		init            ( std::string outputFileName = "", int width = 320, int height = 240, int fps = 25, Cing::GraphicsType format = Cing::RGB );

		virtual void		end             ();
		virtual void		update          () = 0;

		// Video writing control
		virtual void		addFrame        ( Cing::Image& newFrame ) = 0;
		virtual void		finish			() = 0;

		// Query methods
		bool				isValid			() const { return m_bIsValid;   }
		int					getWidth        () const { return m_width;      }
		int					getHeight       () const { return m_height;     }
		int					getFPS          () const { return m_fps;        }

	protected:

		// Attributes
		std::string			m_fileName;					///< Output file name	
		float				m_fps;                		///< Video frames per second
		int					m_width, m_height;    		///< Video resolution

	private:

		// Attributes
		bool				m_bIsValid;	          		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
	};

} // namespace

#endif // _BaseVideoRecorder_H_
