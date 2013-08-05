#include "RTShaderSRSSegmentedLights.h"
#include "OgreShaderFFPRenderState.h"
#include "OgreShaderProgram.h"
#include "OgreShaderParameter.h"
#include "OgreShaderProgramSet.h"
#include "OgreGpuProgram.h"
#include "OgrePass.h"
#include "OgreShaderGenerator.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"
#include "SegmentedDynamicLightManager.h"

#define SL_LIB_PERPIXELLIGHTING					"SegmentedPerPixelLighting"
#define SL_FUNC_TRANSFORMNORMAL					"SL_TransformNormal"
#define SL_FUNC_TRANSFORMPOSITION					"SL_TransformPosition"
#define SL_FUNC_LIGHT_DIRECTIONAL_DIFFUSE			"SL_Light_Directional_Diffuse"
#define SL_FUNC_LIGHT_DIRECTIONAL_DIFFUSESPECULAR	"SL_Light_Directional_DiffuseSpecular"
#define SL_FUNC_LIGHT_AMBIENT_DIFFUSE				"SL_Light_Ambient_Diffuse"
#define SL_FUNC_LIGHT_SEGMENT_TEXTURE_AMBIENT_DIFFUSE	"SL_Light_Segment_Texture_Ambient_Diffuse"
#define SL_FUNC_LIGHT_SEGMENT_DEBUG				"SL_Light_Segment_Debug"

using namespace Ogre;
using namespace Ogre::RTShader;

String RTShaderSRSSegmentedLights::Type = "Segmented_PerPixelLighting";
Light RTShaderSRSSegmentedLights::msBlankLight;


//-----------------------------------------------------------------------
RTShaderSRSSegmentedLights::RTShaderSRSSegmentedLights()
{
	mTrackVertexColourType			= TVC_NONE;	
	mSpecularEnable					= false;
	mUseSegmentedLightTexture		= false;
	m_LightSamplerIndex = -1;

	msBlankLight.setDiffuseColour(ColourValue::Black);
	msBlankLight.setSpecularColour(ColourValue::Black);
	msBlankLight.setAttenuation(0,1,0,0);
}

//-----------------------------------------------------------------------
const String& RTShaderSRSSegmentedLights::getType() const
{
	return Type;
}


//-----------------------------------------------------------------------
int	RTShaderSRSSegmentedLights::getExecutionOrder() const
{
	return FFP_LIGHTING;
}

//-----------------------------------------------------------------------
void RTShaderSRSSegmentedLights::updateGpuProgramsParams(Renderable* rend, Pass* pass, const AutoParamDataSource* source, 
	const LightList* pLightList)
{
	if ((mLightParamsList.size() == 0) && (!mUseSegmentedLightTexture))
		return;

	const Matrix4& matWorld = source->getWorldMatrix();
	Light::LightTypes curLightType = Light::LT_DIRECTIONAL; 
	unsigned int curSearchLightIndex = 0;

	//update spot strength
	float spotIntensity = 1;
	
	// Update per light parameters.
	for (unsigned int i=0; i < mLightParamsList.size(); ++i)
	{
		const LightParams& curParams = mLightParamsList[i];

		if (curLightType != curParams.mType)
		{
			curLightType = curParams.mType;
			curSearchLightIndex = 0;
		}

		Light*		srcLight = NULL;
		Vector4		vParameter;
		ColourValue colour;

		// Search a matching light from the current sorted lights of the given renderable.
		for (unsigned int j = curSearchLightIndex; j < pLightList->size(); ++j)
		{
			if (pLightList->at(j)->getType() == curLightType)
			{				
				srcLight = pLightList->at(j);
				curSearchLightIndex = j + 1;
				break;
			}			
		}

		// No matching light found -> use a blank dummy light for parameter update.
		if (srcLight == NULL)
		{						
			srcLight = &msBlankLight;
		}


		switch (curParams.mType)
		{
		case Light::LT_DIRECTIONAL:

			// Update light direction.
			vParameter = matWorld.transformAffine(srcLight->getAs4DVector(true));
			curParams.mDirection->setGpuParameter(vParameter.ptr(),3,1);
			break;

		case Light::LT_POINT:

			// Update light position.
			vParameter = matWorld.transformAffine(srcLight->getAs4DVector(true));
			curParams.mPosition->setGpuParameter(vParameter.ptr(),3,1);

			// Update light attenuation parameters.
			curParams.mSpotParams->setGpuParameter(Ogre::Vector3(1 / srcLight->getAttenuationRange(),0,0));
			break;

		case Light::LT_SPOTLIGHT:
			{						
				Ogre::Vector3 vec3;
				Ogre::Matrix3 matWorldIT;


				// Update light position.
				vParameter = matWorld.transformAffine(srcLight->getAs4DVector(true));
				curParams.mPosition->setGpuParameter(vParameter.ptr(),3,1);


				// Update light direction.
				source->getInverseTransposeWorldMatrix().extract3x3Matrix(matWorldIT);
				vec3 = matWorldIT * srcLight->getDerivedDirection();
				vec3.normalise();

				vParameter.x = -vec3.x;
				vParameter.y = -vec3.y;
				vParameter.z = -vec3.z;
				vParameter.w = 0.0;
				curParams.mDirection->setGpuParameter(vParameter.ptr(),3,1);

				// Update spotlight parameters.
				Real phi   = Math::Cos(srcLight->getSpotlightOuterAngle().valueRadians() * 0.5f);
				Real theta = Math::Cos(srcLight->getSpotlightInnerAngle().valueRadians() * 0.5f);
				
				vec3.x = 1 / srcLight->getAttenuationRange();
				vec3.y = phi;
				vec3.z = 1 / (theta - phi);

				curParams.mSpotParams->setGpuParameter(vec3);
			}
			break;
		}

		float lightIntensity = 1;
		if (curParams.mType == Light::LT_SPOTLIGHT)
		{
			lightIntensity = spotIntensity;
		}

		// Update diffuse colour.
		colour = srcLight->getDiffuseColour() * lightIntensity;
		if ((mTrackVertexColourType & TVC_DIFFUSE) == 0)
		{
			colour = colour * pass->getDiffuse();
		}
		curParams.mDiffuseColour->setGpuParameter(colour.ptr(),3,1);	

		// Update specular colour if need to.
		if  ((mSpecularEnable) && (curParams.mType == Light::LT_DIRECTIONAL))
		{
			// Update diffuse colour.
			colour = srcLight->getSpecularColour() * lightIntensity;
			if ((mTrackVertexColourType & TVC_SPECULAR) == 0)
			{
				colour = colour * pass->getSpecular();
			}
			curParams.mSpecularColour->setGpuParameter(colour.ptr(),3,1);		
		}																			
	}

	if (mUseSegmentedLightTexture)
	{
		unsigned int indexStart = 0, indexEnd = 0;
		Ogre::Vector4 lightBounds; 
		SegmentedDynamicLightManager::getSingleton().getLightListRange(rend, lightBounds, indexStart, indexEnd);
		mPSLightTextureIndexLimit->setGpuParameter(Ogre::Vector2((Ogre::Real)indexStart, (Ogre::Real)indexEnd));
		mPSLightTextureLightBounds->setGpuParameter(lightBounds);

		if (m_LightSamplerIndex != -1)
		{
			Ogre::TextureUnitState* pLightTexture = pass->getTextureUnitState(m_LightSamplerIndex);
			const Ogre::String& textureName = SegmentedDynamicLightManager::getSingleton().getSDLTextureName();
			if (textureName != pLightTexture->getTextureName())
			{
				pLightTexture->setTextureName(textureName, Ogre::TEX_TYPE_2D);
			}
		}
	}
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::resolveParameters(ProgramSet* programSet)
{
	if (false == resolveGlobalParameters(programSet))
		return false;

	if (false == resolvePerLightParameters(programSet))
		return false;

	return true;
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::resolveGlobalParameters(ProgramSet* programSet)
{
	Program* vsProgram = programSet->getCpuVertexProgram();
	Program* psProgram = programSet->getCpuFragmentProgram();
	Function* vsMain = vsProgram->getEntryPointFunction();
	Function* psMain = psProgram->getEntryPointFunction();


	// Resolve world IT matrix.
	mWorldITMatrix = vsProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_INVERSE_TRANSPOSE_WORLD_MATRIX, 0);
	if (mWorldITMatrix.get() == NULL)		
		return false;	

	// Get surface ambient colour if need to.
	if ((mTrackVertexColourType & TVC_AMBIENT) == 0)
	{		
		mDerivedAmbientLightColour = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_DERIVED_AMBIENT_LIGHT_COLOUR, 0);
		if (mDerivedAmbientLightColour.get() == NULL)		
			return false;
	}
	else
	{
		mLightAmbientColour = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR, 0);
		if (mLightAmbientColour.get() == NULL)		
			return false;	

		mSurfaceAmbientColour = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_SURFACE_AMBIENT_COLOUR, 0);
		if (mSurfaceAmbientColour.get() == NULL)		
			return false;	

	}

	// Get surface diffuse colour if need to.
	if ((mTrackVertexColourType & TVC_DIFFUSE) == 0)
	{
		mSurfaceDiffuseColour = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR, 0);
		if (mSurfaceDiffuseColour.get() == NULL)		
			return false;	 
	}

	// Get surface specular colour if need to.
	if ((mTrackVertexColourType & TVC_SPECULAR) == 0)
	{
		mSurfaceSpecularColour = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_SURFACE_SPECULAR_COLOUR, 0);
		if (mSurfaceSpecularColour.get() == NULL)		
			return false;	 
	}


	// Get surface emissive colour if need to.
	if ((mTrackVertexColourType & TVC_EMISSIVE) == 0)
	{
		mSurfaceEmissiveColour = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_SURFACE_EMISSIVE_COLOUR, 0);
		if (mSurfaceEmissiveColour.get() == NULL)		
			return false;	 
	}

	// Get derived scene colour.
	mDerivedSceneColour = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR, 0);
	if (mDerivedSceneColour.get() == NULL)		
		return false;

	// Get surface shininess.
	mSurfaceShininess = psProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_SURFACE_SHININESS, 0);
	if (mSurfaceShininess.get() == NULL)		
		return false;

	
	//Check if another SRS already defined a normal in world space to be used
	mPSLocalNormal = psMain->getParameterByContent(psMain->getLocalParameters(), Parameter::SPC_NORMAL_WORLD_SPACE, GCT_FLOAT3);
	if (mPSLocalNormal.get() == NULL)
	{
		//create parameters to fetch the normal from the vertex shader
		
		// Resolve input vertex shader normal.
		mVSInNormal = vsMain->resolveInputParameter(Parameter::SPS_NORMAL, 0, Parameter::SPC_NORMAL_OBJECT_SPACE, GCT_FLOAT3);
		if (mVSInNormal.get() == NULL)
			return false;

		// Resolve output vertex shader normal.
		mVSOutNormal = vsMain->resolveOutputParameter(Parameter::SPS_TEXTURE_COORDINATES, -1, Parameter::SPC_NORMAL_WORLD_SPACE, GCT_FLOAT3);
		if (mVSOutNormal.get() == NULL)
			return false;

		// Resolve input pixel shader normal.
		mPSInNormal = psMain->resolveInputParameter(Parameter::SPS_TEXTURE_COORDINATES, 
			mVSOutNormal->getIndex(), 
			mVSOutNormal->getContent(),
			GCT_FLOAT3);

		if (mPSInNormal.get() == NULL)
			return false;

		mPSLocalNormal = psMain->resolveLocalParameter(Parameter::SPS_TEXTURE_COORDINATES, -1, Parameter::SPC_NORMAL_WORLD_SPACE, GCT_FLOAT3);
	}
	
	const ShaderParameterList& inputParams = psMain->getInputParameters();
	const ShaderParameterList& localParams = psMain->getLocalParameters();

	mPSDiffuse = psMain->getParameterByContent(inputParams, Parameter::SPC_COLOR_DIFFUSE, GCT_FLOAT4);
	if (mPSDiffuse.get() == NULL)
	{
		mPSDiffuse = psMain->getParameterByContent(localParams, Parameter::SPC_COLOR_DIFFUSE, GCT_FLOAT4);
		if (mPSDiffuse.get() == NULL)
			return false;
	}

	mPSOutDiffuse = psMain->resolveOutputParameter(Parameter::SPS_COLOR, 0, Parameter::SPC_COLOR_DIFFUSE, GCT_FLOAT4);
	if (mPSOutDiffuse.get() == NULL)
		return false;

	mPSTempDiffuseColour = psMain->resolveLocalParameter(Parameter::SPS_UNKNOWN, 0, "lPerPixelDiffuse", GCT_FLOAT4);
	if (mPSTempDiffuseColour.get() == NULL)
		return false;

	mVSOutWorldPos = vsMain->resolveOutputParameter(Parameter::SPS_TEXTURE_COORDINATES, -1, Parameter::SPC_POSITION_WORLD_SPACE, GCT_FLOAT3);
	if (mVSOutWorldPos.get() == NULL)
		return false;	

	mPSInWorldPos = psMain->resolveInputParameter(Parameter::SPS_TEXTURE_COORDINATES, 
		mVSOutWorldPos->getIndex(), 
		mVSOutWorldPos->getContent(),
		GCT_FLOAT3);
	if (mPSInWorldPos.get() == NULL)
		return false;

	mWorldMatrix = vsProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_WORLD_MATRIX, 0);
	if (mWorldMatrix.get() == NULL)		
		return false;	

	mVSInPosition = vsMain->resolveInputParameter(Parameter::SPS_POSITION, 0, Parameter::SPC_POSITION_OBJECT_SPACE, GCT_FLOAT4);
	if (mVSInPosition.get() == NULL)
		return false;

	if (mSpecularEnable)
	{
		mPSSpecular = psMain->getParameterByContent(inputParams, Parameter::SPC_COLOR_SPECULAR, GCT_FLOAT4);
		if (mPSSpecular.get() == NULL)
		{
			mPSSpecular = psMain->getParameterByContent(localParams, Parameter::SPC_COLOR_SPECULAR, GCT_FLOAT4);
			if (mPSSpecular.get() == NULL)
				return false;
		}

		mPSTempSpecularColour = psMain->resolveLocalParameter(Parameter::SPS_UNKNOWN, 0, "lPerPixelSpecular", GCT_FLOAT4);
		if (mPSTempSpecularColour.get() == NULL)
			return false;


		mVSInPosition = vsMain->resolveInputParameter(Parameter::SPS_POSITION, 0, Parameter::SPC_POSITION_OBJECT_SPACE, GCT_FLOAT4);
		if (mVSInPosition.get() == NULL)
			return false;

		mWorldMatrix = vsProgram->resolveAutoParameterInt(GpuProgramParameters::ACT_WORLD_MATRIX, 0);
		if (mWorldMatrix.get() == NULL)		
			return false;									
	}

	
	if (mUseSegmentedLightTexture)
	{
		mPSLightTextureIndexLimit = psProgram->resolveParameter(GCT_FLOAT2, -1, (uint16)GPV_PER_OBJECT, "LightTextureIndexLimits");
		mPSLightTextureLightBounds = psProgram->resolveParameter(GCT_FLOAT4, -1, (uint16)GPV_PER_OBJECT, "LightTextureBounds");
		mPSSegmentedLightTexture = psProgram->resolveParameter(Ogre::GCT_SAMPLER2D, m_LightSamplerIndex, (Ogre::uint16)Ogre::GPV_GLOBAL, "segmentedLightTexture");
	}

	return true;
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::resolvePerLightParameters(ProgramSet* programSet)
{
	Program* psProgram = programSet->getCpuFragmentProgram();

	// Resolve per light parameters.
	for (unsigned int i=0; i < mLightParamsList.size(); ++i)
	{		
		switch (mLightParamsList[i].mType)
		{
		case Light::LT_DIRECTIONAL:
			mLightParamsList[i].mDirection = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS, "light_direction_space");
			if (mLightParamsList[i].mDirection.get() == NULL)
				return false;
			break;

		case Light::LT_POINT:
		case Light::LT_SPOTLIGHT:
			mLightParamsList[i].mPosition = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS, "light_position_space");
			if (mLightParamsList[i].mPosition.get() == NULL)
				return false;

			mLightParamsList[i].mDirection = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS, "light_direction_space");
			if (mLightParamsList[i].mDirection.get() == NULL)
				return false;

			mLightParamsList[i].mSpotParams = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS, "spotlight_params");
			if (mLightParamsList[i].mSpotParams.get() == NULL)
				return false;

			break;
		}

		// Resolve diffuse colour.
		if ((mTrackVertexColourType & TVC_DIFFUSE) == 0)
		{
			mLightParamsList[i].mDiffuseColour = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS | (uint16)GPV_GLOBAL, "derived_light_diffuse");
			if (mLightParamsList[i].mDiffuseColour.get() == NULL)
				return false;
		}
		else
		{
			mLightParamsList[i].mDiffuseColour = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS, "light_diffuse");
			if (mLightParamsList[i].mDiffuseColour.get() == NULL)
				return false;
		}	

		if ((mSpecularEnable) && (mLightParamsList[i].mType == Light::LT_DIRECTIONAL))
		{
			// Resolve specular colour.
			if ((mTrackVertexColourType & TVC_SPECULAR) == 0)
			{
				mLightParamsList[i].mSpecularColour = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS | (uint16)GPV_GLOBAL, "derived_light_specular");
				if (mLightParamsList[i].mSpecularColour.get() == NULL)
					return false;
			}
			else
			{
				mLightParamsList[i].mSpecularColour = psProgram->resolveParameter(GCT_FLOAT3, -1, (uint16)GPV_LIGHTS, "light_specular");
				if (mLightParamsList[i].mSpecularColour.get() == NULL)
					return false;
			}						
		}		

	}

	return true;
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::resolveDependencies(ProgramSet* programSet)
{
	Program* vsProgram = programSet->getCpuVertexProgram();
	Program* psProgram = programSet->getCpuFragmentProgram();

	vsProgram->addDependency(FFP_LIB_COMMON);
	vsProgram->addDependency(SL_LIB_PERPIXELLIGHTING);

	psProgram->addDependency(FFP_LIB_COMMON);
	psProgram->addDependency(SL_LIB_PERPIXELLIGHTING);

	return true;
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::addFunctionInvocations(ProgramSet* programSet)
{
	Program* vsProgram = programSet->getCpuVertexProgram();	
	Function* vsMain = vsProgram->getEntryPointFunction();	
	Program* psProgram = programSet->getCpuFragmentProgram();
	Function* psMain = psProgram->getEntryPointFunction();	

	int internalCounter = 0;


	// Add the global illumination functions.
	if (false == addVSInvocation(vsMain, FFP_VS_LIGHTING, internalCounter))
		return false;


	internalCounter = 0;

	// Add the global illumination functions.
	if (false == addPSGlobalIlluminationInvocationBegin(psMain, FFP_PS_COLOUR_BEGIN + 1, internalCounter))
		return false;


	// Add per light functions.
	for (unsigned int i=0; i < mLightParamsList.size(); ++i)
	{		
		if (false == addPSIlluminationInvocation(&mLightParamsList[i], psMain, FFP_PS_COLOUR_BEGIN + 1, internalCounter))
			return false;
	}

	if (mUseSegmentedLightTexture)
	{
		addPSSegmentedTextureLightInvocation(psMain, FFP_PS_COLOUR_BEGIN + 1, internalCounter);
	}


	// Add the global illumination functions.
	if (false == addPSGlobalIlluminationInvocationEnd(psMain, FFP_PS_COLOUR_BEGIN + 1, internalCounter))
		return false;


	// Assign back temporary variables to the ps diffuse and specular components.
	if (false == addPSFinalAssignmentInvocation(psMain, FFP_PS_COLOUR_BEGIN + 1, internalCounter))
		return false;


	return true;
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::addVSInvocation(Function* vsMain, const int groupOrder, int& internalCounter)
{
	FunctionInvocation* curFuncInvocation = NULL;

	if (mVSInNormal.get() != NULL)
	{
		// Transform normal in world space.
		curFuncInvocation = OGRE_NEW FunctionInvocation(SL_FUNC_TRANSFORMNORMAL, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mWorldITMatrix, Operand::OPS_IN);
		curFuncInvocation->pushOperand(mVSInNormal, Operand::OPS_IN);
		curFuncInvocation->pushOperand(mVSOutNormal, Operand::OPS_OUT);	
		vsMain->addAtomInstance(curFuncInvocation);
	}

	// Transform world space position if need to.
	if (mVSOutWorldPos.get() != NULL)
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(SL_FUNC_TRANSFORMPOSITION, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mWorldMatrix, Operand::OPS_IN);
		curFuncInvocation->pushOperand(mVSInPosition, Operand::OPS_IN);
		curFuncInvocation->pushOperand(mVSOutWorldPos, Operand::OPS_OUT);	
		vsMain->addAtomInstance(curFuncInvocation);
	}


	return true;
}


//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::addPSGlobalIlluminationInvocationBegin(Function* psMain, const int groupOrder, int& internalCounter)
{
	FunctionInvocation* curFuncInvocation = NULL;	

	if (mPSInNormal.get())
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, FFP_PS_PRE_PROCESS + 1, internalCounter++); 
		curFuncInvocation->pushOperand(mPSInNormal, Operand::OPS_IN);
		curFuncInvocation->pushOperand(mPSLocalNormal, Operand::OPS_OUT);	
		psMain->addAtomInstance(curFuncInvocation);
	}

	//alpha channel is controlled by the diffuse value
	if (mTrackVertexColourType & TVC_DIFFUSE)
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mPSDiffuse, Operand::OPS_IN, Operand::OPM_W);
		curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_W);	
		psMain->addAtomInstance(curFuncInvocation);
	}
	else
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mDerivedSceneColour, Operand::OPS_IN, Operand::OPM_W);
		curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_W);	
		psMain->addAtomInstance(curFuncInvocation);		
	}

	ParameterPtr pZeroParam = ParameterFactory::createConstParamVector3(Ogre::Vector3::ZERO);
	
	curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, groupOrder, internalCounter++); 
	curFuncInvocation->pushOperand(pZeroParam, Operand::OPS_IN);	
	curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);
	psMain->addAtomInstance(curFuncInvocation);	

	if (mSpecularEnable)
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(pZeroParam, Operand::OPS_IN);	
		curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_OUT, Operand::OPM_XYZ);
		psMain->addAtomInstance(curFuncInvocation);	
	}
	
	return true;
}



//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::addPSGlobalIlluminationInvocationEnd(Function* psMain, const int groupOrder, int& internalCounter)
{
	FunctionInvocation* curFuncInvocation = NULL;	

	// Merge diffuse colour with vertex colour if need to.
	if (mTrackVertexColourType & TVC_DIFFUSE)			
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_MODULATE, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mPSDiffuse, Operand::OPS_IN, Operand::OPM_XYZ);	
		curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, Operand::OPM_XYZ);
		curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);
		psMain->addAtomInstance(curFuncInvocation);
	}

	// Merge specular colour with vertex colour if need to.
	if ((mSpecularEnable == true) && (mTrackVertexColourType & TVC_SPECULAR))
	{							
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_MODULATE, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mPSDiffuse, Operand::OPS_IN, Operand::OPM_XYZ);	
		curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_IN, Operand::OPM_XYZ);
		curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_OUT, Operand::OPM_XYZ);
		psMain->addAtomInstance(curFuncInvocation);
	}


	if ((mTrackVertexColourType & TVC_AMBIENT) == 0 && 
		(mTrackVertexColourType & TVC_EMISSIVE) == 0)
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ADD, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mDerivedSceneColour, Operand::OPS_IN, (Operand::OPM_XYZ));
		curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, (Operand::OPM_XYZ));	
		curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
		psMain->addAtomInstance(curFuncInvocation);		
	}
	else
	{
		if (mTrackVertexColourType & TVC_AMBIENT)
		{
			curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_MODULATE, groupOrder, internalCounter++); 
			curFuncInvocation->pushOperand(mPSDiffuse, Operand::OPS_IN, Operand::OPM_XYZ);			
			curFuncInvocation->pushOperand(mLightAmbientColour, Operand::OPS_IN, Operand::OPM_XYZ);
			curFuncInvocation->pushOperand(mLightAmbientColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);
		
			curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ADD, groupOrder, internalCounter++); 
			curFuncInvocation->pushOperand(mLightAmbientColour, Operand::OPS_IN, Operand::OPM_XYZ);
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, Operand::OPM_XYZ);			
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);
		}
		else
		{
			curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ADD, groupOrder, internalCounter++); 
			curFuncInvocation->pushOperand(mDerivedAmbientLightColour, Operand::OPS_IN, Operand::OPM_XYZ);	
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, Operand::OPM_XYZ);	
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);
		}

		if (mTrackVertexColourType & TVC_EMISSIVE)
		{
			curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ADD, groupOrder, internalCounter++); 
			curFuncInvocation->pushOperand(mPSDiffuse, Operand::OPS_IN, Operand::OPM_XYZ);
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, Operand::OPM_XYZ);
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);
		}
		else
		{
			curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ADD, groupOrder, internalCounter++); 
			curFuncInvocation->pushOperand(mSurfaceEmissiveColour, Operand::OPS_IN, Operand::OPM_XYZ);
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, Operand::OPM_XYZ);
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);
		}		
	}

	if (mSpecularEnable)
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ADD, groupOrder, internalCounter++); 
		curFuncInvocation->pushOperand(mPSSpecular, Operand::OPS_IN);
		curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_IN);	
		curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_OUT);	
		psMain->addAtomInstance(curFuncInvocation);	
	}

	return true;
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::addPSIlluminationInvocation(LightParams* curLightParams, Function* psMain, const int groupOrder, int& internalCounter)
{	
	FunctionInvocation* curFuncInvocation = NULL;	

	
	switch (curLightParams->mType)
	{

	case Light::LT_DIRECTIONAL:			
		if (mSpecularEnable)
		{				
			curFuncInvocation = OGRE_NEW FunctionInvocation(SL_FUNC_LIGHT_DIRECTIONAL_DIFFUSESPECULAR, groupOrder, internalCounter++); 
			curFuncInvocation->pushOperand(mPSLocalNormal, Operand::OPS_IN);
			curFuncInvocation->pushOperand(mPSInWorldPos, Operand::OPS_IN);			
			curFuncInvocation->pushOperand(curLightParams->mDirection, Operand::OPS_IN);
			curFuncInvocation->pushOperand(curLightParams->mDiffuseColour, Operand::OPS_IN);			
			curFuncInvocation->pushOperand(curLightParams->mSpecularColour, Operand::OPS_IN);			
			curFuncInvocation->pushOperand(mSurfaceShininess, Operand::OPS_IN);
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, Operand::OPM_XYZ);	
			curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_IN, Operand::OPM_XYZ);
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);
		}

		else
		{
			curFuncInvocation = OGRE_NEW FunctionInvocation(SL_FUNC_LIGHT_DIRECTIONAL_DIFFUSE, groupOrder, internalCounter++); 			
			curFuncInvocation->pushOperand(mPSLocalNormal, Operand::OPS_IN);
			curFuncInvocation->pushOperand(curLightParams->mDirection, Operand::OPS_IN);
			curFuncInvocation->pushOperand(curLightParams->mDiffuseColour, Operand::OPS_IN);					
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN, Operand::OPM_XYZ);	
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_OUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);	
		}	
		break;

	case Light::LT_POINT:	
	case Light::LT_SPOTLIGHT:
		{
			curFuncInvocation = OGRE_NEW FunctionInvocation(SL_FUNC_LIGHT_AMBIENT_DIFFUSE, groupOrder, internalCounter++); 						
			curFuncInvocation->pushOperand(mPSLocalNormal, Operand::OPS_IN);
			curFuncInvocation->pushOperand(mPSInWorldPos, Operand::OPS_IN);
			curFuncInvocation->pushOperand(curLightParams->mPosition, Operand::OPS_IN);
			curFuncInvocation->pushOperand(curLightParams->mDirection, Operand::OPS_IN);
			curFuncInvocation->pushOperand(curLightParams->mSpotParams, Operand::OPS_IN);
			curFuncInvocation->pushOperand(curLightParams->mDiffuseColour, Operand::OPS_IN);					
			curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_INOUT, Operand::OPM_XYZ);	
			psMain->addAtomInstance(curFuncInvocation);	
		}
		break;
	}

	return true;
}

bool RTShaderSRSSegmentedLights::addPSSegmentedTextureLightInvocation(Function* psMain, const int groupOrder, int& internalCounter)
{
	float invWidth = 1.0f / (float)SegmentedDynamicLightManager::getSingleton().getTextureWidth();
	float invHeight = 1.0f / (float)SegmentedDynamicLightManager::getSingleton().getTextureHeight();
	ParameterPtr paramInvWidth = ParameterFactory::createConstParamFloat(invWidth);
	ParameterPtr paramInvHeight = ParameterFactory::createConstParamFloat(invHeight);

	FunctionInvocation* curFuncInvocation = NULL;	
	curFuncInvocation = OGRE_NEW FunctionInvocation(SL_FUNC_LIGHT_SEGMENT_TEXTURE_AMBIENT_DIFFUSE, groupOrder, internalCounter++); 						
	curFuncInvocation->pushOperand(mPSLocalNormal, Operand::OPS_IN);
	curFuncInvocation->pushOperand(mPSInWorldPos, Operand::OPS_IN);
	curFuncInvocation->pushOperand(mPSSegmentedLightTexture, Operand::OPS_IN);
	curFuncInvocation->pushOperand(mPSLightTextureIndexLimit, Operand::OPS_IN);
	curFuncInvocation->pushOperand(mPSLightTextureLightBounds, Operand::OPS_IN);
	curFuncInvocation->pushOperand(paramInvWidth, Operand::OPS_IN);
	curFuncInvocation->pushOperand(paramInvHeight, Operand::OPS_IN);					
	curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_INOUT, Operand::OPM_XYZ);	
	psMain->addAtomInstance(curFuncInvocation);	

	if (SegmentedDynamicLightManager::getSingleton().isDebugMode())
	{
		ParameterPtr psOutColor = psMain->resolveOutputParameter(Parameter::SPS_COLOR, -1, Parameter::SPC_COLOR_DIFFUSE, GCT_FLOAT4);

		FunctionInvocation* curDebugFuncInvocation = NULL;
		curDebugFuncInvocation = OGRE_NEW FunctionInvocation(SL_FUNC_LIGHT_SEGMENT_DEBUG, FFP_PS_COLOUR_END + 1, internalCounter++); 						
		curDebugFuncInvocation->pushOperand(mPSLocalNormal, Operand::OPS_IN);
		curDebugFuncInvocation->pushOperand(mPSInWorldPos, Operand::OPS_IN);
		curDebugFuncInvocation->pushOperand(mPSSegmentedLightTexture, Operand::OPS_IN);
		curDebugFuncInvocation->pushOperand(mPSLightTextureIndexLimit, Operand::OPS_IN);
		curDebugFuncInvocation->pushOperand(mPSLightTextureLightBounds, Operand::OPS_IN);
		curDebugFuncInvocation->pushOperand(paramInvWidth, Operand::OPS_IN);
		curDebugFuncInvocation->pushOperand(paramInvHeight, Operand::OPS_IN);	

		curDebugFuncInvocation->pushOperand(psOutColor, Operand::OPS_INOUT, Operand::OPM_XYZ);	
		psMain->addAtomInstance(curDebugFuncInvocation);	
	}

	return true;
}


//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::addPSFinalAssignmentInvocation( Function* psMain, const int groupOrder, int& internalCounter )
{
	FunctionInvocation* curFuncInvocation;

	curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, FFP_PS_COLOUR_BEGIN + 1, internalCounter++); 								
	curFuncInvocation->pushOperand(mPSTempDiffuseColour, Operand::OPS_IN);	
	curFuncInvocation->pushOperand(mPSDiffuse, Operand::OPS_OUT);	
	psMain->addAtomInstance(curFuncInvocation);	

	curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, FFP_PS_COLOUR_BEGIN + 1, internalCounter++); 								
	curFuncInvocation->pushOperand(mPSDiffuse, Operand::OPS_IN);	
	curFuncInvocation->pushOperand(mPSOutDiffuse, Operand::OPS_OUT);	
	psMain->addAtomInstance(curFuncInvocation);

	if (mSpecularEnable)
	{
		curFuncInvocation = OGRE_NEW FunctionInvocation(FFP_FUNC_ASSIGN, FFP_PS_COLOUR_BEGIN + 1, internalCounter++); 
		curFuncInvocation->pushOperand(mPSTempSpecularColour, Operand::OPS_IN);
		curFuncInvocation->pushOperand(mPSSpecular, Operand::OPS_OUT);			
		psMain->addAtomInstance(curFuncInvocation);	
	}

	return true;
}


//-----------------------------------------------------------------------
void RTShaderSRSSegmentedLights::copyFrom(const SubRenderState& rhs)
{
	const RTShaderSRSSegmentedLights& rhsLighting = static_cast<const RTShaderSRSSegmentedLights&>(rhs);

	mUseSegmentedLightTexture = rhsLighting.mUseSegmentedLightTexture;
	
	int lightCount[3];

	rhsLighting.getLightCount(lightCount);
	setLightCount(lightCount);
}

//-----------------------------------------------------------------------
bool RTShaderSRSSegmentedLights::preAddToRenderState(const RenderState* renderState, Pass* srcPass, Pass* dstPass)
{
	if (srcPass->getLightingEnabled() == false)
		return false;

	mUseSegmentedLightTexture = SegmentedDynamicLightManager::getSingleton().isActive();
	setTrackVertexColourType(srcPass->getVertexColourTracking());			

	if (srcPass->getShininess() > 0.0 &&
		srcPass->getSpecular() != ColourValue::Black)
	{
		setSpecularEnable(true);
	}
	else
	{
		setSpecularEnable(false);	
	}

	
	int lightCount[3];
	renderState->getLightCount(lightCount);
	setLightCount(lightCount);

	if (mUseSegmentedLightTexture)
	{
		const_cast<RenderState*>(renderState)->setLightCountAutoUpdate(false);

		Ogre::TextureUnitState* pLightTexture = dstPass->createTextureUnitState();
		pLightTexture->setTextureName(SegmentedDynamicLightManager::getSingleton().getSDLTextureName(), Ogre::TEX_TYPE_2D);
		pLightTexture->setTextureFiltering(Ogre::TFO_NONE);
		m_LightSamplerIndex = dstPass->getNumTextureUnitStates() - 1;
	}


	return true;
}

//-----------------------------------------------------------------------
void RTShaderSRSSegmentedLights::setLightCount(const int lightCount[3])
{
	mLightParamsList.clear();
	//Set always to have one single directional lights
	LightParams curParams;
	curParams.mType = Light::LT_DIRECTIONAL;
	mLightParamsList.push_back(curParams);

	for (int type=0; type < 3; ++type)
	{
		for (int i=0; i < lightCount[type]; ++i)
		{
			
			if (type == 0)
				curParams.mType = Light::LT_POINT;
			//else if (type == 1)
			//	curParams.mType = Light::LT_DIRECTIONAL;
			else if (type == 2)
				curParams.mType = Light::LT_SPOTLIGHT;		

			if ((!mUseSegmentedLightTexture) || (curParams.mType == Light::LT_DIRECTIONAL))
			{
				mLightParamsList.push_back(curParams);
			}
		}
	}			
}

//-----------------------------------------------------------------------
void RTShaderSRSSegmentedLights::getLightCount(int lightCount[3]) const
{
	lightCount[0] = 0;
	lightCount[1] = 0;
	lightCount[2] = 0;

	for (unsigned int i=0; i < mLightParamsList.size(); ++i)
	{
		const LightParams curParams = mLightParamsList[i];

		if (curParams.mType == Light::LT_POINT)
			lightCount[0]++;
		else if (curParams.mType == Light::LT_DIRECTIONAL)
			lightCount[1]++;
		else if (curParams.mType == Light::LT_SPOTLIGHT)
			lightCount[2]++;
	}
}


//-----------------------------------------------------------------------
const String& RTShaderSRSSegmentedLightsFactory::getType() const
{
	return RTShaderSRSSegmentedLights::Type;
}

//-----------------------------------------------------------------------
SubRenderState*	RTShaderSRSSegmentedLightsFactory::createInstance(ScriptCompiler* compiler, 
		PropertyAbstractNode* prop, Pass* pass, SGScriptTranslator* translator)
{
	if (prop->name == "lighting_stage")
	{
		if(prop->values.size() == 1)
		{
			String modelType;

			if(false == SGScriptTranslator::getString(prop->values.front(), &modelType))
			{
				compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, prop->line);
				return NULL;
			}

			if (modelType == "per_pixel")
			{
				return createOrRetrieveInstance(translator);
			}
		}		
	}

	return NULL;
}

//-----------------------------------------------------------------------
void RTShaderSRSSegmentedLightsFactory::writeInstance(MaterialSerializer* ser, SubRenderState* subRenderState, 
	Pass* srcPass, Pass* dstPass)
{
	ser->writeAttribute(4, "lighting_stage");
	ser->writeValue("per_pixel");
}

//-----------------------------------------------------------------------
SubRenderState*	RTShaderSRSSegmentedLightsFactory::createInstanceImpl()
{
	return OGRE_NEW RTShaderSRSSegmentedLights;
}

