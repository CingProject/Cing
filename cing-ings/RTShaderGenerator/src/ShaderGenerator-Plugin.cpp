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

#include "ShaderGenerator-Plugin.h"
#include "ShaderGeneratorTechniqueResolverListener.h"
#include "framework/UserAppGlobals.h"
#include "graphics/Window.h"

using namespace Cing;

/**
 * Clears attributes, but does not init the shader generation system
 */
ShaderGenerator::ShaderGenerator():	
	Cing::Plugin( "ShaderGenerator" ),
	m_shaderGenerator(NULL), m_materialMgrListener(NULL), m_curLightingModel(PerVertexLighting), m_perPixelFogEnable(false),
	m_specularEnable(false), m_shaderLibsPath(""), m_valid(false), m_sceneManager(NULL), m_viewPort(NULL)
{
	setPluginInitTime(INIT_BEFORE_RESOURCE_INIT);
	setPluginEndTime(END_AFTER_USER);

	// Some default settings
	m_dirLightCastShadows	= true;
	m_spotLightCastShadows	= false;
	m_pointLightCastShadows = false;
	m_selfShadows			= true;
	m_sizePerShadowTexture	= 1024;

}

/**
 * Clears attributes and releases the system resources
 */
ShaderGenerator::~ShaderGenerator()
{
	end();

	// Clear vars
	m_shaderGenerator = NULL;
	m_materialMgrListener = NULL;
	m_sceneManager = NULL;
	m_viewPort = NULL;
	m_shaderLibsPath = "";
	m_valid = false;
}

/**
 * Initializs the Shader Generator System. While it is enabled, this system will create shader based techniques for
 * all the materials in the scene, adding the global techniques set in the syste, like per pixel lighting or PSSM shadows
 * that (when enabled) will be added to all the materials when creating the shader based techniques for them.
 *
 * @return true of the system is correctly initialized, false otherwise.
 */
bool ShaderGenerator::init()
{
	LOG_ENTER_FUNCTION; 

	// Check the main Ogre Scene manager has not been created yet, we cannot init the system
	if ( !ogreSceneManager && !m_sceneManager )
	{
		LOG_ERROR("ShaderGenerator Initialization Error: there is no main scene manager created, nor set for the Shader Generator.");
		m_valid = false;
		LOG_EXIT_FUNCTION; 
		return false;
	}

	// Now, check there is a valid window and viewport set to the generator, or that at least the main app viewport is valid
	if ( !m_viewPort && !((appWindow != NULL) && (appWindow->isValid() == true) && (appWindow->getMainViewport() != NULL) ) )
	{
		LOG_ERROR("ShaderGenerator Initialization Error: there is no viewport set to the shader generator, nor a main viewport app created.");
		m_valid = false;
		LOG_EXIT_FUNCTION; 
		return false;
	}

	// if no scene manager has been set, use the main global one
	if ( !m_sceneManager )
		m_sceneManager = ogreSceneManager;

	// if no viewport has been set, use the main global one as well
	if ( !m_viewPort )
		m_viewPort = appWindow->getMainViewport();
	
	// Init the Ogre RT Shader System
	if (Ogre::RTShader::ShaderGenerator::initialize())
	{
		// Grab the shader generator pointer.
		m_shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

		// Add the shader libs resource location.
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ResourceManager::libDataPath + "/RTShaderLib", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ResourceManager::libDataPath + "/RTShaderLib/materials", "FileSystem");

		// Set shader cache path.
		std::string cachePath = ResourceManager::libDataPath + "/RTShaderLib/cache/";
		m_shaderGenerator->setShaderCachePath(cachePath);      

		// Set the scene manager.
		m_shaderGenerator->addSceneManager(m_sceneManager);
		LOG("ShaderGenerator Initialized Correctly");
		
		m_materialMgrListener = new ShaderGeneratorTechniqueResolverListener(m_shaderGenerator);				
		Ogre::MaterialManager::getSingleton().addListener(m_materialMgrListener);

		// Set the scheme so that this viewport is rendered using the shader generated techniques for all the materials
		m_viewPort->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

		// invalidate it to make sure all the materials are re-generated
		m_shaderGenerator->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

		m_valid = true;
	} 
	// Error with RT Shader System init
	else
	{
		LOG("Error initializing ShaderGenerator System. Real time shaders will not be created");
		m_valid = false;
	}

	LOG_EXIT_FUNCTION; 
	return m_valid;
}

/**
 * Releases the resources used by the Shader Generator
 */
void ShaderGenerator::end()
{
	LOG_ENTER_FUNCTION; 

	// If it's not valid, do nothing
	if ( !m_valid )
		return;

	delete m_materialMgrListener;
	m_materialMgrListener = NULL;

	m_valid = false;
	LOG_EXIT_FUNCTION; 
}

/**
 * Adds a scene manager that will also use the RT Shader system to render its objects
 * @param sceneManager to add
 */
void ShaderGenerator::addSceneManager( Ogre::SceneManager* sceneManager )
{
	LOG_ENTER_FUNCTION; 

	// check system is ok
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::addSceneManager Error ShaderGenerator is not valid or not correctly initialized." );
		LOG_EXIT_FUNCTION; 
		return;
	}
	
	if ( m_shaderGenerator )
		m_shaderGenerator->addSceneManager(sceneManager);

	LOG_EXIT_FUNCTION; 
}

/**
 * Removes a scene manager that will stop being bond to the RT Shader system to render its objects
 * @param sceneManager to remove from the list
 */
void ShaderGenerator::removeSceneManager( Ogre::SceneManager* sceneManager )
{
	LOG_ENTER_FUNCTION; 

	// check system is ok
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::removeSceneManager Error ShaderGenerator is not valid or not correctly initialized." );
		LOG_EXIT_FUNCTION; 
		return;
	}

	if ( m_shaderGenerator )
		m_shaderGenerator->removeSceneManager(sceneManager);

LOG_EXIT_FUNCTION; 
}

/**
 * Sets a lighting model that will be applied to all materials
 * @param newModel New lighting model that will be applied (previous model will be removed)
 * @param textureName Only applicable to normal mapping. Should be the name of the normal map texture to use.
 * @return true if the lighting model was correctly applied, false otheriwse
 */
bool ShaderGenerator::setLightingModel( ShaderGeneratorLightingModel newModel, const std::string& textureName /*= ""*/ )
{
	LOG_ENTER_FUNCTION; 

	// check system is ok
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::setLightingModel Error ShaderGenerator is not valid or not correctly initialized." );
		LOG_EXIT_FUNCTION; 
		return false;
	}

	// Get the global render state to apply the new lighting model to it
	Ogre::RTShader::RenderState* globalRenderState = m_shaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	if ( !globalRenderState )
	{
		LOG_ERROR( "ShaderGenerator::setLightingModel Error getting global render state. New Lighting model will not be applied." );
		LOG_EXIT_FUNCTION; 
		return false;
	}	

	// Set the lighting model to the render state
	setLightingModelToRenderState(globalRenderState, newModel, textureName);

	// Invalidate the scheme in order to re-generate all shaders based technique related to this scheme.
	m_shaderGenerator->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	LOG_EXIT_FUNCTION; 

	// all good
	return true;
}

/**
 * Sets a lighting model that will be applied to a specific material
 * @param newModel New lighting model that will be applied (previous model will be removed)
 * @param materialName Material that will use the new lighting model
 * @param textureName Only applicable to normal mapping. Should be the name of the normal map texture to use.
 * @param pasIndex Advanced optional parameter that allows to apply the new lighting model only to a specific pass for the material
 * @return true if the lighting model was correctly applied, false otheriwse
 */
bool ShaderGenerator::setLightingModelToMaterial( ShaderGeneratorLightingModel newModel, const std::string& materialName, const std::string& textureName /*= ""*/, unsigned int pasIndex /*= 0*/ )
{
	LOG_ENTER_FUNCTION; 

	// check system is ok
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::setLightingModel Error ShaderGenerator is not valid or not correctly initialized." );
		LOG_EXIT_FUNCTION; 
		return false;
	}

	// Create the shader based technique of this material.
	bool success = m_shaderGenerator->createShaderBasedTechnique(materialName, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	// Setup custom shader sub render states according to current setup.
	if (success)
	{					
		Ogre::MaterialPtr curMaterial	= Ogre::MaterialManager::getSingleton().getByName(materialName);
		Ogre::Pass* curPass				= curMaterial->getTechnique(0)->getPass(0);

		if (m_specularEnable)
		{
			curPass->setSpecular(Ogre::ColourValue::White);
			curPass->setShininess(32.0);
		}
		else
		{
			curPass->setSpecular(Ogre::ColourValue::Black);
			curPass->setShininess(0.0);
		}
		

		// Grab the first pass render state. 
		// NOTE: For more complicated samples iterate over the passes and build each one of them as desired.
		Ogre::RTShader::RenderState* renderState = m_shaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, materialName, pasIndex);

		// Get the global render state to apply the new lighting model to it
		if ( !renderState )
		{
			LOG_ERROR( "ShaderGenerator::setLightingModel Error getting material render state. New Lighting model will not be applied." );
			LOG_EXIT_FUNCTION; 
			return false;
		}	

		// Remove all sub render states.
		renderState->reset();

		// Set the lighting model to the render state
		Ogre::RTShader::SubRenderState* subRenderState = setLightingModelToRenderState(renderState, newModel, textureName);
		if ( !subRenderState )
			success = false;

		// Invalidate the scheme in order to re-generate all shaders based technique related to this scheme.
		m_shaderGenerator->invalidateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, materialName);

		LOG_EXIT_FUNCTION; 
		return success;
	}

	LOG_EXIT_FUNCTION; 

	// all good
	return success;
}

/**
 * Sets a shadowing model that will be applied to all materials
 * @param newModel New shadow model that will be applied (previous model will be removed)
 * @return true if the shadow model was correctly applied, false otheriwse
 */
bool ShaderGenerator::setShadowModel( ShaderGeneratorShadowModel newModel )
{
	LOG_ENTER_FUNCTION; 

	// check system is ok
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::setShadowModel Error ShaderGenerator is not valid or not correctly initialized." );
		LOG_EXIT_FUNCTION; 
		return false;
	}

	// Get the global render state to apply the new lighting model to it
	Ogre::RTShader::RenderState* globalRenderState = m_shaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	if ( !globalRenderState )
	{
		LOG_ERROR( "ShaderGenerator::setShadowModel Error getting global render state. Per Pixel Lighting will not be applied." );
		LOG_EXIT_FUNCTION; 
		return false;
	}	

	// first, clear previous applied shadow models
	clearShadowRenderStates();

	// then, apply new shadow model
	switch ( newModel )
	{
		case NoShadows:
			m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
		break;

		case StencilAdditive:
			m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		break;

		case StencilModulative:
			m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
		break;

		case PSSMTextureAdditive:
			setupPSSMShadows(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
		break;

		case PSSMTextureModulative:
			setupPSSMShadows(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);
		break;

	};

	// Invalidate the scheme in order to re-generate all shaders based technique related to this scheme.
	m_shaderGenerator->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	LOG_EXIT_FUNCTION; 
	
	// all good
	return true;
}

/**
 * Invalidates a material so that it is regenerated again with the Real Time Shader System
 * @param materialName Name of the material to be re-generated
 */
void ShaderGenerator::invalidateMaterial( const std::string& materialName )
{
	LOG_ENTER_FUNCTION; 

	// check system is ok
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::invalidateMaterial Error ShaderGenerator is not valid or not correctly initialized." );
		return;
	}

	m_shaderGenerator->invalidateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, materialName);
	LOG_EXIT_FUNCTION; 
}


/**
 * Sets a lighting model to a RT Shader system render state, which could be the global (to apply the lighting to all materials in the scene)
 * or could be the render state of a specific material (to apply the lightintg model to that specific material only)
 * @param renderState The Render state to apply the lighting model to
 * @param newModel New Lighting model to apply to the render state
 * @param textureName Name of the texture to use, in case the lighting model needs one (like for Normal Mapping)
 * @return the new subrenderstate added for this lighting model, or NULL if there was an error
 */
Ogre::RTShader::SubRenderState* ShaderGenerator::setLightingModelToRenderState( Ogre::RTShader::RenderState* renderState, ShaderGeneratorLightingModel newModel, const std::string& textureName /*= ""*/ )
{
	LOG_ENTER_FUNCTION; 

	// check system is ok
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::setLightingModelToRenderState Error ShaderGenerator is not valid or not correctly initialized." );
		LOG_EXIT_FUNCTION; 
		return false;
	}

	// Now check render state pointer
	if ( !renderState )
	{
		LOG_ERROR( "ShaderGenerator::setLightingModelToRenderState Error renderState received is NULL." );
		LOG_EXIT_FUNCTION; 
		return false;
	}

	// first, clear previous applied lighting models
	clearLightingRenderStates();

	// vars used in the switch
	Ogre::RTShader::SubRenderState* newLightModel		= NULL;
	Ogre::RTShader::NormalMapLighting* normalMapSubRS	= NULL;

	// then, apply new lighting model
	switch ( newModel )
	{
		case PerVertexLighting:
			//Create a global subrender state for per pixel lighting. This will be applied to all materials in the scene.
			newLightModel = m_shaderGenerator->createSubRenderState(Ogre::RTShader::FFPLighting::Type);
			if ( !newLightModel )
			{
				LOG_ERROR( "ShaderGenerator::setLightingModel Error creating PerVertexLighting sub render state. Per Vertex Lighting will not be applied." );
				LOG_EXIT_FUNCTION; 
				return false;
			}
			
			// Add the subrender state, and we're done
			renderState->addTemplateSubRenderState(newLightModel);    
		break;

		case PerPixelLighting:
			//Create a global subrender state for per pixel lighting. This will be applied to all materials in the scene.
			newLightModel = m_shaderGenerator->createSubRenderState(Ogre::RTShader::PerPixelLighting::Type);
			if ( !newLightModel )
			{
				LOG_ERROR( "ShaderGenerator::setLightingModel Error creating PerPixelLighting sub render state. Per Pixel Lighting will not be applied." );
				LOG_EXIT_FUNCTION; 
				return false;
			}
			
			// Add the subrender state, all working.
			renderState->addTemplateSubRenderState(newLightModel);    
		break;

		case NormalMapLightingTangentSpace:
			//Create a global subrender state for normal mapping. This will be applied to all materials in the scene.
			newLightModel = m_shaderGenerator->createSubRenderState(Ogre::RTShader::NormalMapLighting::Type);
			if ( !newLightModel )
			{
				LOG_ERROR( "ShaderGenerator::setLightingModel Error creating NormalMapLighting sub render state. Normal mapping Lighting will not be applied." );
				LOG_EXIT_FUNCTION; 
				return false;
			}
			
			normalMapSubRS = static_cast<Ogre::RTShader::NormalMapLighting*>(newLightModel);			
			normalMapSubRS->setNormalMapSpace(Ogre::RTShader::NormalMapLighting::NMS_TANGENT);
			normalMapSubRS->setNormalMapTextureName(textureName);	

			// Add the subrender state, all working.
			renderState->addTemplateSubRenderState(normalMapSubRS);    
		break;

		case NormalMapLightingObjectSpace:
			//Create a global subrender state for normal mapping. This will be applied to all materials in the scene.
			newLightModel = m_shaderGenerator->createSubRenderState(Ogre::RTShader::NormalMapLighting::Type);
			if ( !newLightModel )
			{
				LOG_ERROR( "ShaderGenerator::setLightingModel Error creating NormalMapLighting sub render state. Normal mapping Lighting will not be applied." );
				LOG_EXIT_FUNCTION; 
				return false;
			}
			
			normalMapSubRS = static_cast<Ogre::RTShader::NormalMapLighting*>(newLightModel);			
			normalMapSubRS->setNormalMapSpace(Ogre::RTShader::NormalMapLighting::NMS_OBJECT);
			normalMapSubRS->setNormalMapTextureName(textureName);	

			// Add the subrender state, all working.
			renderState->addTemplateSubRenderState(normalMapSubRS);    
		break;
	};

	LOG_EXIT_FUNCTION; 

	return newLightModel;
}


/**
 * Removes all the lighting models from the Render state. 
 */
void ShaderGenerator::clearLightingRenderStates()
{
	LOG_ENTER_FUNCTION; 

	// Grab the scheme render state.												
	Ogre::RTShader::RenderState* schemRenderState = m_shaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	// Find lighting related render states and remove them from the list
	const Ogre::RTShader::SubRenderStateList& subRenderStateList = schemRenderState->getTemplateSubRenderStateList();
	Ogre::RTShader::SubRenderStateListConstIterator it = subRenderStateList.begin();
	Ogre::RTShader::SubRenderStateListConstIterator itEnd = subRenderStateList.end();

	for (; it != itEnd; ++it)
	{
		Ogre::RTShader::SubRenderState* curSubRenderState = *it;

		// Remove per vertex lighting
		if (curSubRenderState->getType() == Ogre::RTShader::FFPLighting::Type)
		{
			schemRenderState->removeTemplateSubRenderState(*it);
			break;
		}
		// Remove per pixel lighting
		else if (curSubRenderState->getType() == Ogre::RTShader::PerPixelLighting::Type)
		{
			schemRenderState->removeTemplateSubRenderState(*it);
			break;
		}
		// Remove normal mapping
		else if (curSubRenderState->getType() == Ogre::RTShader::NormalMapLighting::Type)
		{
			schemRenderState->removeTemplateSubRenderState(*it);
			break;
		}
	}

	LOG_EXIT_FUNCTION; 
}

/**
 * Removes all the shadow models from the Render state. 
 */
void ShaderGenerator::clearShadowRenderStates()
{
	LOG_ENTER_FUNCTION; 

	// Grab the scheme render state.												
	Ogre::RTShader::RenderState* schemRenderState = m_shaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	// Find shadow related render states and remove them from the list
	const Ogre::RTShader::SubRenderStateList& subRenderStateList = schemRenderState->getTemplateSubRenderStateList();
	Ogre::RTShader::SubRenderStateListConstIterator it = subRenderStateList.begin();
	Ogre::RTShader::SubRenderStateListConstIterator itEnd = subRenderStateList.end();

	for (; it != itEnd; ++it)
	{
		Ogre::RTShader::SubRenderState* curSubRenderState = *it;

		// Remove PSSM lighting
		if (curSubRenderState->getType() == Ogre::RTShader::IntegratedPSSM3::Type)
		{
			schemRenderState->removeTemplateSubRenderState(*it);
			break;
		}
	}

	LOG_EXIT_FUNCTION; 
}

/**
 * Configures the Parallel Split Shadow maps with default parameters and 3 splits
 * @param technique Shadow technique to use. Could be: Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED or Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED
 */
void ShaderGenerator::setupPSSMShadows( Ogre::ShadowTechnique technique )
{
	LOG_ENTER_FUNCTION; 

	// Make sure the scene manager pointer is good
	if ( !isValid() )
	{
		LOG_ERROR( "ShaderGenerator::setupPSSMShadows. Error: Scene Manager pointer is NULL" );
		return;
	}

	m_sceneManager->setShadowTechnique(technique);

	// Enable ligh types
	int lightTypeEnabledCount = 0;
	if ( m_dirLightCastShadows )
	{
		m_sceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
		++lightTypeEnabledCount;
	}
	if ( m_pointLightCastShadows )
	{
		m_sceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 3);
		++lightTypeEnabledCount;
	}
	if ( m_spotLightCastShadows )
	{
		m_sceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 3);
		++lightTypeEnabledCount;
	}

	m_sceneManager->setShadowTextureSettings(m_sizePerShadowTexture, lightTypeEnabledCount*3, Ogre::PF_FLOAT32_R);
	m_sceneManager->setShadowTextureSelfShadow(m_selfShadows);
	//m_sceneManager->setShadowCasterRenderBackFaces(false);

	// Set up caster material - this is just a standard depth/shadow map caster
	m_sceneManager->setShadowTextureCasterMaterial("PSSM/shadow_caster");
	//m_sceneManager->setShadowTextureCasterMaterial("Ogre/shadow/depth/caster");

	// Disable fog on the caster pass.
	Ogre::MaterialPtr passCaterMaterial = Ogre::MaterialManager::getSingleton().getByName("PSSM/shadow_caster");
	Ogre::Pass* pssmCasterPass = passCaterMaterial->getTechnique(0)->getPass(0);
	pssmCasterPass->setFog(true);

	// Clipping planes to use for the shadow camera
	float nearClipPlane = ogreCamera? ogreCamera->getNearClipDistance() * 0.5f: 5.0f;
	float farClipPlane	= ogreCamera? ogreCamera->getFarClipDistance(): 3000.0f;

	// Create the shadow camera setup
	Ogre::PSSMShadowCameraSetup* pssmSetup = new Ogre::PSSMShadowCameraSetup();

	// For now we use default split points, but likely they should be adjusted depedning on the camara/light settings
	// This would be the code to do it
	//pssmSetup->calculateSplitPoints(3, nearClipPlane, farClipPlane);
	//pssmSetup->setSplitPadding(10);
	//pssmSetup->setOptimalAdjustFactor(0, 2);
	//pssmSetup->setOptimalAdjustFactor(1, 1);
	//pssmSetup->setOptimalAdjustFactor(2, 0.5);

	// Set the shadow camera to the main scene manager (shared pointer, no need to delete it)
	m_sceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));

	Ogre::RTShader::SubRenderState* subRenderState = m_shaderGenerator->createSubRenderState(Ogre::RTShader::IntegratedPSSM3::Type);   
	Ogre::RTShader::IntegratedPSSM3* pssm3SubRenderState = static_cast<Ogre::RTShader::IntegratedPSSM3*>(subRenderState);
	const Ogre::PSSMShadowCameraSetup::SplitPointList& srcSplitPoints = pssmSetup->getSplitPoints();
	Ogre::RTShader::IntegratedPSSM3::SplitPointList dstSplitPoints;

	for (unsigned int i=0; i < srcSplitPoints.size(); ++i)
	{
		dstSplitPoints.push_back(srcSplitPoints[i]);
	}

	pssm3SubRenderState->setSplitPoints(dstSplitPoints);

	Ogre::RTShader::RenderState* schemRenderState = m_shaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	schemRenderState->addTemplateSubRenderState(subRenderState);

	LOG_EXIT_FUNCTION; 
}