# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# For each target create a dummy rule so the target does not have to exist
$(SRCROOT)/boost/lib/libboost_thread-mt.a:
$(SRCROOT)/boost/lib/libboost_date_time-mt.a:
$(SRCROOT)/boost/lib/libboost_system-mt.a:
$(SRCROOT)/boost/lib/libboost_chrono-mt.a:
/opt/local/lib/libOIS.a:


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.Sample_BezierPatch.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_BezierPatch.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_BezierPatch.dylib


PostBuild.Sample_CameraTrack.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_CameraTrack.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_CameraTrack.dylib


PostBuild.Sample_Character.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Character.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Character.dylib


PostBuild.Sample_Compositor.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Compositor.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Compositor.dylib


PostBuild.Sample_DeferredShading.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_DeferredShading.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_DeferredShading.dylib


PostBuild.Sample_DualQuaternion.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_DualQuaternion.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_DualQuaternion.dylib


PostBuild.Sample_DynTex.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_DynTex.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_DynTex.dylib


PostBuild.Sample_EndlessWorld.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_EndlessWorld.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_EndlessWorld.dylib


PostBuild.Sample_FacialAnimation.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_FacialAnimation.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_FacialAnimation.dylib


PostBuild.Sample_Fresnel.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Fresnel.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Fresnel.dylib


PostBuild.Sample_Grass.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Grass.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Grass.dylib


PostBuild.Sample_Instancing.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Instancing.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Instancing.dylib


PostBuild.Sample_Lighting.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Lighting.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Lighting.dylib


PostBuild.Sample_MeshLod.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_MeshLod.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_MeshLod.dylib


PostBuild.Sample_NewInstancing.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_NewInstancing.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_NewInstancing.dylib


PostBuild.Sample_Ocean.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Ocean.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Ocean.dylib


PostBuild.Sample_ParticleFX.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_ParticleFX.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_ParticleFX.dylib


PostBuild.Sample_ShaderSystem.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_ShaderSystem.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_ShaderSystem.dylib


PostBuild.Sample_ShaderSystemMultiLight.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_ShaderSystemMultiLight.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_ShaderSystemMultiLight.dylib


PostBuild.Sample_ShaderSystemTexturedFog.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_ShaderSystemTexturedFog.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_ShaderSystemTexturedFog.dylib


PostBuild.Sample_Shadows.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Shadows.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Shadows.dylib


PostBuild.Sample_SkeletalAnimation.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_SkeletalAnimation.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_SkeletalAnimation.dylib


PostBuild.Sample_SkyBox.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_SkyBox.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_SkyBox.dylib


PostBuild.Sample_SkyDome.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_SkyDome.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_SkyDome.dylib


PostBuild.Sample_SkyPlane.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_SkyPlane.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_SkyPlane.dylib


PostBuild.Sample_Smoke.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Smoke.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Smoke.dylib


PostBuild.Sample_SphereMapping.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_SphereMapping.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_SphereMapping.dylib


PostBuild.Sample_Terrain.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Terrain.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Terrain.dylib


PostBuild.Sample_TextureFX.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_TextureFX.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_TextureFX.dylib


PostBuild.Sample_TextureArray.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_TextureArray.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_TextureArray.dylib


PostBuild.Sample_Transparency.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Transparency.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Transparency.dylib


PostBuild.Sample_VolumeTex.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_VolumeTex.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_VolumeTex.dylib


PostBuild.Sample_VolumeCSG.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_VolumeCSG.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_VolumeCSG.dylib


PostBuild.Sample_VolumeTerrain.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_VolumeTerrain.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_VolumeTerrain.dylib


PostBuild.Sample_Water.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Water.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Water.dylib


PostBuild.Sample_BSP.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_BSP.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_BSP.dylib


PostBuild.Sample_Isosurf.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Isosurf.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Isosurf.dylib


PostBuild.Sample_ParticleGS.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_ParticleGS.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_ParticleGS.dylib


PostBuild.Sample_SSAO.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_SSAO.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_SSAO.dylib


PostBuild.Sample_Tesselation.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Tesselation.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Tesselation.dylib


PostBuild.Sample_CelShading.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_CelShading.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_CelShading.dylib


PostBuild.Sample_CubeMapping.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_CubeMapping.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_CubeMapping.dylib


PostBuild.Sample_Dot3Bump.Debug:
$(SRCROOT)/lib/macosx/Debug/Sample_Dot3Bump.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Debug/Sample_Dot3Bump.dylib


PostBuild.SampleBrowser.Debug:
$(SRCROOT)/bin/Debug/SampleBrowser.app/Contents/MacOS/SampleBrowser:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/bin/Debug/SampleBrowser.app/Contents/MacOS/SampleBrowser


PostBuild.Sample_BezierPatch.Release:
$(SRCROOT)/lib/macosx/Release/Sample_BezierPatch.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_BezierPatch.dylib


PostBuild.Sample_CameraTrack.Release:
$(SRCROOT)/lib/macosx/Release/Sample_CameraTrack.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_CameraTrack.dylib


PostBuild.Sample_Character.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Character.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Character.dylib


PostBuild.Sample_Compositor.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Compositor.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Compositor.dylib


PostBuild.Sample_DeferredShading.Release:
$(SRCROOT)/lib/macosx/Release/Sample_DeferredShading.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_DeferredShading.dylib


PostBuild.Sample_DualQuaternion.Release:
$(SRCROOT)/lib/macosx/Release/Sample_DualQuaternion.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_DualQuaternion.dylib


PostBuild.Sample_DynTex.Release:
$(SRCROOT)/lib/macosx/Release/Sample_DynTex.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_DynTex.dylib


PostBuild.Sample_EndlessWorld.Release:
$(SRCROOT)/lib/macosx/Release/Sample_EndlessWorld.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_EndlessWorld.dylib


PostBuild.Sample_FacialAnimation.Release:
$(SRCROOT)/lib/macosx/Release/Sample_FacialAnimation.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_FacialAnimation.dylib


PostBuild.Sample_Fresnel.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Fresnel.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Fresnel.dylib


PostBuild.Sample_Grass.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Grass.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Grass.dylib


PostBuild.Sample_Instancing.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Instancing.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Instancing.dylib


PostBuild.Sample_Lighting.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Lighting.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Lighting.dylib


PostBuild.Sample_MeshLod.Release:
$(SRCROOT)/lib/macosx/Release/Sample_MeshLod.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_MeshLod.dylib


PostBuild.Sample_NewInstancing.Release:
$(SRCROOT)/lib/macosx/Release/Sample_NewInstancing.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_NewInstancing.dylib


PostBuild.Sample_Ocean.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Ocean.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Ocean.dylib


PostBuild.Sample_ParticleFX.Release:
$(SRCROOT)/lib/macosx/Release/Sample_ParticleFX.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_ParticleFX.dylib


PostBuild.Sample_ShaderSystem.Release:
$(SRCROOT)/lib/macosx/Release/Sample_ShaderSystem.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_ShaderSystem.dylib


PostBuild.Sample_ShaderSystemMultiLight.Release:
$(SRCROOT)/lib/macosx/Release/Sample_ShaderSystemMultiLight.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_ShaderSystemMultiLight.dylib


PostBuild.Sample_ShaderSystemTexturedFog.Release:
$(SRCROOT)/lib/macosx/Release/Sample_ShaderSystemTexturedFog.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_ShaderSystemTexturedFog.dylib


PostBuild.Sample_Shadows.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Shadows.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Shadows.dylib


PostBuild.Sample_SkeletalAnimation.Release:
$(SRCROOT)/lib/macosx/Release/Sample_SkeletalAnimation.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_SkeletalAnimation.dylib


PostBuild.Sample_SkyBox.Release:
$(SRCROOT)/lib/macosx/Release/Sample_SkyBox.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_SkyBox.dylib


PostBuild.Sample_SkyDome.Release:
$(SRCROOT)/lib/macosx/Release/Sample_SkyDome.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_SkyDome.dylib


PostBuild.Sample_SkyPlane.Release:
$(SRCROOT)/lib/macosx/Release/Sample_SkyPlane.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_SkyPlane.dylib


PostBuild.Sample_Smoke.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Smoke.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Smoke.dylib


PostBuild.Sample_SphereMapping.Release:
$(SRCROOT)/lib/macosx/Release/Sample_SphereMapping.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_SphereMapping.dylib


PostBuild.Sample_Terrain.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Terrain.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Terrain.dylib


PostBuild.Sample_TextureFX.Release:
$(SRCROOT)/lib/macosx/Release/Sample_TextureFX.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_TextureFX.dylib


PostBuild.Sample_TextureArray.Release:
$(SRCROOT)/lib/macosx/Release/Sample_TextureArray.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_TextureArray.dylib


PostBuild.Sample_Transparency.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Transparency.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Transparency.dylib


PostBuild.Sample_VolumeTex.Release:
$(SRCROOT)/lib/macosx/Release/Sample_VolumeTex.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_VolumeTex.dylib


PostBuild.Sample_VolumeCSG.Release:
$(SRCROOT)/lib/macosx/Release/Sample_VolumeCSG.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_VolumeCSG.dylib


PostBuild.Sample_VolumeTerrain.Release:
$(SRCROOT)/lib/macosx/Release/Sample_VolumeTerrain.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_VolumeTerrain.dylib


PostBuild.Sample_Water.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Water.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Water.dylib


PostBuild.Sample_BSP.Release:
$(SRCROOT)/lib/macosx/Release/Sample_BSP.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_BSP.dylib


PostBuild.Sample_Isosurf.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Isosurf.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Isosurf.dylib


PostBuild.Sample_ParticleGS.Release:
$(SRCROOT)/lib/macosx/Release/Sample_ParticleGS.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_ParticleGS.dylib


PostBuild.Sample_SSAO.Release:
$(SRCROOT)/lib/macosx/Release/Sample_SSAO.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_SSAO.dylib


PostBuild.Sample_Tesselation.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Tesselation.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Tesselation.dylib


PostBuild.Sample_CelShading.Release:
$(SRCROOT)/lib/macosx/Release/Sample_CelShading.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_CelShading.dylib


PostBuild.Sample_CubeMapping.Release:
$(SRCROOT)/lib/macosx/Release/Sample_CubeMapping.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_CubeMapping.dylib


PostBuild.Sample_Dot3Bump.Release:
$(SRCROOT)/lib/macosx/Release/Sample_Dot3Bump.dylib:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/macosx/Release/Sample_Dot3Bump.dylib


PostBuild.SampleBrowser.Release:
$(SRCROOT)/bin/Release/SampleBrowser.app/Contents/MacOS/SampleBrowser:\
	$(SRCROOT)/boost/lib/libboost_thread-mt.a\
	$(SRCROOT)/boost/lib/libboost_date_time-mt.a\
	$(SRCROOT)/boost/lib/libboost_system-mt.a\
	$(SRCROOT)/boost/lib/libboost_chrono-mt.a\
	/opt/local/lib/libOIS.a
	/bin/rm -f $(SRCROOT)/bin/Release/SampleBrowser.app/Contents/MacOS/SampleBrowser


