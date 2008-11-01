/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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
*/

#include "BlobTracker.h"

// Common
#include "common/Exception.h"

// Graphics
#include "graphics/Image.h"

namespace ComputerVision
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BlobTracker::BlobTracker():
	m_bIsValid            ( false )
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
void BlobTracker::update( const Graphics::Image& inImage )
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
		if (m_trackedBlobs[i].isActive == true) {
			m_trackedBlobs[i].energy -=0.5;			
		};

	};

	float dD        = 5;   //distancia en pixels a la que se considera el mismo blob
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
					//callBack(j);					
					break;
				};
			};
		};
	};
};

/**
 * @internal
 * @brief Set maximum number of blobs
 * 
 * @param[in] maxBlobs
 */
void  BlobTracker::setMaxBlobs( float maxBlobs ){
	
	BlobFinder::setMaxBlobs( maxBlobs );

	m_trackedBlobs.clear();
	for (int i=0; i<maxBlobs;i++){
		m_trackedBlobs.push_back( TrackedBlob(i) );
	};
}

} // namespace ComputerVision