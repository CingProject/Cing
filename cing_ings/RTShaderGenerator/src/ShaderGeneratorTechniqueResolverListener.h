/*
  This source file is part of the Cing project
  For the latest info, see http:///<www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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

// Precompiled headers
#include "Cing-Precompiled.h"
#include "OgreColourValue.h"


/** 
 This sub class the material manager listener class and when a target scheme callback
 is invoked with the shader generator scheme it tries to create an equivalent shader
 based technique based on the default technique of the given material.
*/
class ShaderGeneratorTechniqueResolverListener : public Ogre::MaterialManager::Listener
{
public:

	ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* pShaderGenerator)
	{
		m_shaderGenerator	= pShaderGenerator;	
		m_specularEnable	= true;
		m_specularColor		= Ogre::ColourValue::White;
		m_shininess			= 32.0;

	}

	/** This is the hook point where shader based technique will be created.
	 It will be called whenever the Ogre material manager won't find appropriate technique
	 that satisfy the target scheme name. If the scheme name is out target RT Shader System
	 scheme name we will try to create shader generated technique for it. 
	*/
	virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, 
		const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, 
		const Ogre::Renderable* rend)
	{	
		LOG_ENTER_FUNCTION; 
		Ogre::Technique* generatedTech = NULL;

		// Case this is the default shader generator scheme.
		if (schemeName == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
		{
			bool techniqueCreated;

			// Create shader generated technique for this material.
			techniqueCreated = m_shaderGenerator->createShaderBasedTechnique( originalMaterial->getName(), 
																			  Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
																			  schemeName);	

			// Case technique registration succeeded.
			if (techniqueCreated)
			{
				// Enable specular?
				Ogre::MaterialPtr curMaterial = Ogre::MaterialManager::getSingleton().getByName(originalMaterial->getName());
				Ogre::Pass* curPass = NULL;
				if ( curMaterial->getTechnique(0) && curMaterial->getTechnique(0)->getPass(0) )
					curPass = curMaterial->getTechnique(0)->getPass(0);

				if (curPass && m_specularEnable)
				{
					curPass->setSpecular(m_specularColor);
					curPass->setShininess(m_shininess);
				}
				else if ( curPass )
				{
					curPass->setSpecular(Ogre::ColourValue::Black);
					curPass->setShininess(0.0);
				}

				// Force creating the shaders for the generated technique.
				m_shaderGenerator->validateMaterial(schemeName, originalMaterial->getName());
				
				// Grab the generated technique.
				Ogre::Material::TechniqueIterator itTech = originalMaterial->getTechniqueIterator();

				while (itTech.hasMoreElements())
				{
					Ogre::Technique* curTech = itTech.getNext();

					if (curTech->getSchemeName() == schemeName)
					{
						generatedTech = curTech;
						break;
					}
				}				
			}
		}

		LOG_EXIT_FUNCTION; 
		return generatedTech;
	}

protected:	
	Ogre::RTShader::ShaderGenerator*	m_shaderGenerator;	///< The shader generator instance.		
	bool								m_specularEnable;	///< If true, all materials will have specular enabled
	Ogre::ColourValue					m_specularColor;	///< Color of the specular shine
	double								m_shininess;		///< Shininess is the 'inverse of specular color splattering' coefficient. If this is big (e.g : 64) you get a very tiny dot with a quite strong color (on round surface).  If this is 0, you get a simple color layer (the dot has become very wide). Default 32

};
