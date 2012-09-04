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

#ifndef ShaderGenerator_H
#define ShaderGenerator_H

// Include library to link here to make easier using the plugin (only windows for now).


#include "RTShaderSystem/OgreRTShaderSystem.h"
#include "common/CommonUtilsIncludes.h"

#include "framework/CingPlugin.h"

// TODO: pragma to include lib in visual studio
#if defined(WIN32)
#ifdef _DEBUG
		#pragma comment(lib,"OgreRTShaderSystem_d.lib")
#else
		#pragma comment(lib,"OgreRTShaderSystem.lib")
#endif
#endif 

///< Lighting models.
enum ShaderGeneratorLightingModel
{
	PerVertexLighting,
	PerPixelLighting,
	NormalMapLightingTangentSpace,
	NormalMapLightingObjectSpace,		
};

///< Shadow models.
enum ShaderGeneratorShadowModel
{

	NoShadows,				///< No shadows (removes shadows if they were set previously)
	
	// Stencil based shadows. Simple, run in lower graphics cards, but are less detailed/realistic
	StencilAdditive,		///< Additive Stecil Shadows. This is here for ease of use, but it is actually using default stencil shadows in Ogre, not the real time shader system. Stencil shadows require edge list on the meshes
	StencilModulative,		///< Modulative Stecil Shadows. This is here for ease of use, but it is actually using default stencil shadows in Ogre, not the real time shader system. Stencil shadows require edge list on the meshes

	// Texture based shadows. More advanced, require higher graphics card. 
	// Tip: Make sure your light's derivedDirection is normalized
	PSSMTextureAdditive,	///< Addivite Texture Shadows using Parallel Split Shadow Maps technique. More info here: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch10.html
	PSSMTextureModulative	///< Modulative Texture Shadows using Parallel Split Shadow Maps technique. More info here: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch10.html
};



/**
 * System to create shaders in real time based on defined material properties.
 * Allows to enable features such as per pixel lighting or PSSM shadows on a global basis 
 * to be applied to all entities/materials, or on a per instance basis.
 * It wraps the functionality of the Ogre Real Time Shader System.
 */
class ShaderGenerator: public Cing::Plugin
{
public:

	ShaderGenerator();
	~ShaderGenerator();

	// Scene manager should be set before the pluing init is called
	void setSceneManger	(Ogre::SceneManager* sceneManager ) { m_sceneManager = sceneManager; } ///< If not called, the global ogreScenenManager will be used
	bool init			();
	void end			();
	bool isValid		() const { return m_valid && (m_sceneManager != NULL) && (m_shaderGenerator != NULL); }

	// Scene manager(s) related
	void addSceneManager( Ogre::SceneManager* sceneManager );
	void removeSceneManager( Ogre::SceneManager* sceneManager );
	
	// Change render states for generated shaders
	bool setLightingModel			( ShaderGeneratorLightingModel newModel, const std::string& textureName = "" );
	bool setLightingModelToMaterial	( ShaderGeneratorLightingModel newModel, const std::string& materialName, const std::string& textureName = "", unsigned int pasIndex = 0 );
	bool setShadowModel				( ShaderGeneratorShadowModel newModel );

	// Shadow related controls (NOTE: these should be called before enabling shadows)
	void enableDirLightCastShadows	( bool enable ) 		{ m_dirLightCastShadows = enable; }
	void enablePointLightCastShadows( bool enable )			{ m_pointLightCastShadows = enable; }
	void enableSpotLightCastShadows	( bool enable )			{ m_spotLightCastShadows = enable; }
	void enableSelfShadow			( bool enable )			{ m_selfShadows = enable; }
	void setSizePerShadowTexture	( unsigned int size )	{ m_sizePerShadowTexture = size; }

	// Material generation
	void invalidateMaterial			( const std::string& materialName );

	// Advanced direct access to shader generator. Useful to do things not wraped by this class
	Ogre::RTShader::ShaderGenerator* getShaderGenerator() { return m_shaderGenerator; }

private:
	// private methods
	Ogre::RTShader::SubRenderState* setLightingModelToRenderState( Ogre::RTShader::RenderState* renderState, ShaderGeneratorLightingModel newModel, const std::string& textureName = "" );
	void clearLightingRenderStates		();
	void clearShadowRenderStates		();
	void setupPSSMShadows				( Ogre::ShadowTechnique technique );


	bool								m_valid;				///< True when the system has been correctly initialized		

	// Ogre objects
	Ogre::SceneManager*					m_sceneManager;			///< The scene manager with the objects that will use the shader generator system
	Ogre::Viewport*						m_viewPort;				///< Viewport to which the materials generated by this system will be applied
	Ogre::RTShader::ShaderGenerator*	m_shaderGenerator;		///< The Shader generator instance.
	Ogre::MaterialManager::Listener*	m_materialMgrListener;


	// System features
	ShaderGeneratorLightingModel		m_curLightingModel;		///< The current lighting model.
	bool								m_perPixelFogEnable;	///< When true the RTSS will do per pixel fog calculations.
	bool								m_specularEnable;		///< The current specular state.	
	
	//bool								m_reflectionMapEnable;	///< The current reflection map effect state.
	//RTShader::SubRenderStateFactory*	m_reflectionMapFactory;	///< The custom reflection map shader extension factory.
	//RTShader::SubRenderState*			m_reflectionMapSubRS;	///< The reflection map sub render state.

	// Shadow settings
	bool								m_dirLightCastShadows;	///< If true, directional lights will be able to cast shadows (if the light has castShadows enabled). Default is true.
	bool								m_pointLightCastShadows;///< If true, point lights will be able to cast shadows (if the light has castShadows enabled). Default is false.
	bool								m_spotLightCastShadows;	///< If true, spot lights will be able to cast shadows (if the light has castShadows enabled). Default is false.
	bool								m_selfShadows;			///< If true, self shadowing will happen. Default is true.
	unsigned int						m_sizePerShadowTexture;	///< Size for each shadow texture (3 of this size will be used per light type). Default 1024.

	std::string							m_shaderLibsPath;		///< The path of the RTShader Libs (where created shader cache will be stored)
};

#endif