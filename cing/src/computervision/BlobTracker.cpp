/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

// Precompiled headers
#include "Cing-Precompiled.h"


#include "BlobTracker.h"

// Common
#include "common/Exception.h"

// Graphics
#include "graphics/Image.h"

namespace Cing
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BlobTracker::BlobTracker():
	m_bIsValid            ( false ),
	m_callback						( NULL)
{
	// Init TrackedBlobs
	for (int i=0; i< (int) BlobFinder::DEFAULT_MAX_BLOBS;i++){
		m_trackedBlobs.push_back( TrackedBlob(i) );
	};

	m_bIsValid = true;
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
BlobTracker::~BlobTracker()
{
  // Release resources
  end();
}


/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void BlobTracker::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;
  
	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Find the blobs and track them to assign unique ids
 * 
 * @param[in] inImage image where the blobs will be searched
 */
void BlobTracker::update( const Image& inImage )
{
	// Check valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to compute blobs, with the BlobTracker not initialized. Init method should be called" );

	BlobFinder::update( inImage );

	for (int i=0; i<(int)BlobFinder::m_maxBlobs; i++){
		//un blob ha perdido toda su energia y se desactiva
		if (m_trackedBlobs[i].energy < 0.0 && m_trackedBlobs[i].isActive == true) {
			m_trackedBlobs[i].energy = 0.0;
			m_trackedBlobs[i].blob.center.x = -100;
			m_trackedBlobs[i].blob.center.y = -100;
			m_trackedBlobs[i].isActive = false;
		};
		//restamos energia y mantenemos la posicion anterior + TODO:(la velocidad!???)
		if (m_trackedBlobs[i].isActive == true) 
			m_trackedBlobs[i].energy -=0.5;			
	};

	float dD        = 5;    //distancia en pixels a la que se considera el mismo blob
	float maxEnergy = 10.0;	//max energia de cada tracked blob
	float dist      = 0.0;
	//para cada blob capturado por el contourFinder...
	for (int i=0; i<BlobFinder::getNumBlobs();i++){
		// comprobamos si este blob ya existía, para ello
		// miramos si la distancia entre éste, y todos los
		// blobs que están siendo seguidos es menor del 
		// umbral dD
		bool isFound = false;

		for (int j=0; j<(int)BlobFinder::m_maxBlobs;j++){
			//calculamos la distancia 
			dist = BlobFinder::m_blobs[i].center.distance(m_trackedBlobs[j].blob.center);
			if (dist < dD){
				// si es menor, ya existía, así que actualizamos sus datos
				m_trackedBlobs[j].blob.center = BlobFinder::m_blobs[i].center;
				/*m_trackedBlobs[j].velocity.x = (trackedBlobs[j].vel.x + (trackedBlobs[j].centroid.x - trackedBlobs[j].oldPos.x))/2.0;
				m_trackedBlobs[j].velocity.y = (trackedBlobs[j].vel.y + (trackedBlobs[j].centroid.y - trackedBlobs[j].oldPos.y))/2.0;*/
				m_trackedBlobs[j].isActive = true;
				if (m_trackedBlobs[j].energy < maxEnergy) m_trackedBlobs[j].energy +=1.0;
				isFound = true;
				break;
			};
		};

		//si no es encontrado, activamos uno inactivo para seguirlo
		if (isFound == false) {
			for (int j=0; j<(int)BlobFinder::m_maxBlobs;j++){//esto debería optimizarse!!!
				//encontramos uno que no esté siendo usado
				if(m_trackedBlobs[j].isActive == false){
					m_trackedBlobs[j].blob.center = BlobFinder::m_blobs[i].center;
					m_trackedBlobs[j].isActive = true;
					if (m_trackedBlobs[j].energy < maxEnergy) m_trackedBlobs[j].energy +=1.0;
					// TODO: revisar
					if (m_callback != NULL)
						m_callback;
					//callBack(j);					
					break;
				};
			};
		};
	};
	m_callback;
};

/**
 * @internal
 * @brief Set maximum number of blobs
 * 
 * @param[in] maxBlobs
 */
void  BlobTracker::setMaxBlobs( int maxBlobs ){
	
	BlobFinder::setMaxBlobs( maxBlobs );

	m_trackedBlobs.clear();
	for (int i=0; i<maxBlobs;i++){
		m_trackedBlobs.push_back( TrackedBlob(i) );
	};
}

} // namespace Cing