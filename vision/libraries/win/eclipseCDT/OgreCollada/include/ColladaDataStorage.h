/*
-----------------------------------------------------------------------------
This source file is part of OgreCollada (http://ogrecollada.sf.net)

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

#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <Ogre.h>

#include <map>
#include <vector>
#include "ColladaAnimationClip.h"

namespace OgreCollada
{
	/** A data storage used during the preprocessing of COLLADA documents.
	 * @author Van Aarde Krynauw
	 */
	class DataStorage
	{
		public:
			/** Default constructor */
			DataStorage();
			/** Destructor */
			~DataStorage();
			
			typedef std::map<int, std::vector<int> > SingleToMultiMap;
			typedef std::map< std::pair<std::string, std::string>, SingleToMultiMap > StringPairMap;
			typedef std::map< std::string, AnimationClip* > AnimationClipMap;
			
			/** Get a reference to the SingleToMultiMap variable that maps COLLADA vertex indices to Ogre vertex indices
			 */
			StringPairMap& getRemappedIndices();
			
			/** Create an animation clip.
			 * @param clipName The name of the animation clip.
			   @remarks It is your responsibility to make sure the you don't add animations with the same name as previously
			            added animations. Previous animations will simply be overwritten.
			   @return A pointer to the created animation clip.
			*/
			AnimationClip* createAnimationClip(std::string clipName);
			
			/** Retrieve an animation clip.
			 * @param clipName The name of the animation clip.
			   @return A pointer to the animation clip. NULL is returned if the animation clip does not exist.
			 */
			AnimationClip* getAnimationClip(std::string clipName);
			
			/** Retrieve animation clip map.
			   @remarks Do not modify this structure. In due course a better method may be devised to return 
			            the animation clip map but until then use this function, with care.
			   @return A pointer to the map containing the animation clips.
			 */
			AnimationClipMap& getAnimationClipMap();
			
		private:
			//Remapping of the COLLADA indices to Ogre indices
			StringPairMap m_RemappedIndices;
			AnimationClipMap mAnimationClips;
	};
}

#endif
