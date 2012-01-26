cmake_minimum_required(VERSION 2.6)

project(rt3dapp)

set(SHARED "../../shared")
set(APP "../source")
set(COMPPATH "${SHARED}/Entity")
set(CLANMATH "${SHARED}/ClanLib-2.0/Sources/Core/Math")
set(BOOSTSIGNALSPATH "${SHARED}/util/boost/libs/signals/src")
set(JPGSRC "${SHARED}/Irrlicht/source/Irrlicht/jpeglib")
set(PNGSRC "${SHARED}/Irrlicht/source/Irrlicht/libpng")
set(IRRSRC "${SHARED}/Irrlicht/source/Irrlicht")
set(IRRMESH "${IRRSRC}")
set(IRRSCENE "${IRRSRC}")

add_definitions(-DRTLINUX -DBOOST_ALL_NO_LIB -DC_GL_MODE -DRT_USE_SDL_AUDIO -D_IRR_STATIC_LIB_)
add_definitions(-DDEBUG -D_DEBUG)

include_directories("${APP}")
include_directories("${SHARED}")
include_directories("${SHARED}/ClanLib-2.0/Sources")
include_directories("${SHARED}/util/boost")
include_directories("${SHARED}/Irrlicht/include")

add_executable(rt3dapp "${APP}/App.cpp" "${APP}/GUI/DebugMenu.cpp" "${APP}/GUI/MainMenu.cpp" "${APP}/GUI/Map3Menu.cpp" "${APP}/GUI/MapMenu.cpp" "${APP}/GUI/Mesh3DMenu.cpp" "${APP}/GUI/TerrainMenu.cpp" "${APP}/Component/FPSControlComponent.cpp"
"${SHARED}/PlatformSetup.cpp" "${SHARED}/linux/LinuxUtils.cpp" "${SHARED}/linux/LinuxMain.cpp"
"${SHARED}/Audio/AudioManager.cpp" "${SHARED}/Audio/AudioManagerSDL.cpp"
"${SHARED}/GUI/RTFont.cpp"
"${SHARED}/Manager/Console.cpp" "${SHARED}/Manager/GameTimer.cpp" "${SHARED}/Manager/MessageManager.cpp" "${SHARED}/Manager/ResourceManager.cpp" "${SHARED}/Manager/VariantDB.cpp"
"${SHARED}/Math/rtPlane.cpp" "${SHARED}/Math/rtRect.cpp"
"${SHARED}/Renderer/RenderBatcher.cpp" "${SHARED}/Renderer/SoftSurface.cpp" "${SHARED}/Renderer/Surface.cpp" "${SHARED}/Renderer/SurfaceAnim.cpp"
"${SHARED}/util/CRandom.cpp" "${SHARED}/util/GLESUtils.cpp" "${SHARED}/util/MathUtils.cpp" "${SHARED}/util/MiscUtils.cpp" "${SHARED}/util/RenderUtils.cpp" "${SHARED}/util/ResourceUtils.cpp" "${SHARED}/util/Variant.cpp"
"${SHARED}/BaseApp.cpp"
"${SHARED}/util/unzip/unzip.c" "${SHARED}/util/unzip/ioapi.c" "${SHARED}/util/TextScanner.cpp" "${SHARED}/Entity/EntityUtils.cpp"
"${SHARED}/Network/NetHTTP.cpp" "${SHARED}/Network/NetSocket.cpp" "${SHARED}/Network/NetUtils.cpp" "${SHARED}/FileSystem/StreamingInstance.cpp" "${SHARED}/FileSystem/StreamingInstanceZip.cpp" "${SHARED}/FileSystem/StreamingInstanceFile.cpp" "${SHARED}/FileSystem/FileSystem.cpp" "${SHARED}/FileSystem/FileSystemZip.cpp" "${SHARED}/FileSystem/FileManager.cpp"
"${COMPPATH}/Entity.cpp" "${COMPPATH}/Component.cpp" "${COMPPATH}/Button2DComponent.cpp" "${COMPPATH}/FilterInputComponent.cpp" "${COMPPATH}/FocusInputComponent.cpp" "${COMPPATH}/FocusRenderComponent.cpp" "${COMPPATH}/FocusUpdateComponent.cpp" "${COMPPATH}/HTTPComponent.cpp" "${COMPPATH}/InputTextRenderComponent.cpp" "${COMPPATH}/InterpolateComponent.cpp" "${COMPPATH}/OverlayRenderComponent.cpp" "${COMPPATH}/ProgressBarComponent.cpp" "${COMPPATH}/RectRenderComponent.cpp" "${COMPPATH}/ScrollBarRenderComponent.cpp" "${COMPPATH}/ScrollComponent.cpp" "${COMPPATH}/TapSequenceDetectComponent.cpp" "${COMPPATH}/TextBoxRenderComponent.cpp" "${COMPPATH}/TextRenderComponent.cpp" "${COMPPATH}/TouchStripComponent.cpp" "${COMPPATH}/TrailRenderComponent.cpp" "${COMPPATH}/TyperComponent.cpp" "${COMPPATH}/UnderlineRenderComponent.cpp" "${COMPPATH}/TouchHandlerComponent.cpp" "${COMPPATH}/SelectButtonWithCustomInputComponent.cpp" "${COMPPATH}/CustomInputComponent.cpp" "${COMPPATH}/SliderComponent.cpp" "${COMPPATH}/EmitVirtualKeyComponent.cpp"
"${BOOSTSIGNALSPATH}/connection.cpp" "${BOOSTSIGNALSPATH}/named_slot_map.cpp" "${BOOSTSIGNALSPATH}/signal_base.cpp" "${BOOSTSIGNALSPATH}/slot.cpp" "${BOOSTSIGNALSPATH}/trackable.cpp" 
"${CLANMATH}/angle.cpp" "${CLANMATH}/mat3.cpp" "${CLANMATH}/mat4.cpp" "${CLANMATH}/rect.cpp" "${CLANMATH}/vec2.cpp" "${CLANMATH}/vec3.cpp" "${CLANMATH}/vec4.cpp"
"${SHARED}/Irrlicht/IrrlichtManager.cpp"
"${IRRSRC}/CAttributes.cpp" "${IRRSRC}/CBoneSceneNode.cpp" "${IRRSRC}/CColorConverter.cpp" "${IRRSRC}/CDefaultSceneNodeAnimatorFactory.cpp" "${IRRSRC}/CDefaultSceneNodeFactory.cpp" "${IRRSRC}/CDepthBuffer.cpp" "${IRRSRC}/CDummyTransformationSceneNode.cpp" "${IRRSRC}/CEmptySceneNode.cpp" "${IRRSRC}/CFPSCounter.cpp" "${IRRSRC}/CGeometryCreator.cpp" "${IRRSRC}/CLightSceneNode.cpp" "${IRRSRC}/CLogger.cpp" "${IRRSRC}/CMemoryFile.cpp" "${IRRSRC}/CMeshCache.cpp" "${IRRSRC}/CMeshManipulator.cpp" "${IRRSRC}/CMeshSceneNode.cpp" "${IRRSRC}/COCTLoader.cpp" "${IRRSRC}/COctreeSceneNode.cpp" "${IRRSRC}/CSkinnedMesh.cpp" "${IRRSRC}/CTextSceneNode.cpp" "${IRRSRC}/CTriangleBBSelector.cpp" "${IRRSRC}/CTriangleSelector.cpp" "${IRRSRC}/COctreeTriangleSelector.cpp" "${IRRSRC}/CVideoModeList.cpp" "${IRRSRC}/CVolumeLightSceneNode.cpp" "${IRRSRC}/CWaterSurfaceSceneNode.cpp" "${IRRSRC}/Irrlicht.cpp" "${IRRSRC}/irrXML.cpp" "${IRRSRC}/os.cpp" "${IRRSRC}/CMetaTriangleSelector.cpp"
"${IRRSRC}/CCameraSceneNode.cpp" "${IRRSRC}/CSceneNodeAnimatorCameraFPS.cpp"
"${IRRSRC}/CIrrDeviceStub.cpp" "${IRRSRC}/CIrrDeviceWin32.cpp"
"${IRRSRC}/CFileList.cpp" "${IRRSRC}/CFileSystem.cpp" "${IRRSRC}/CLimitReadFile.cpp" "${IRRSRC}/CMountPointReader.cpp" "${IRRSRC}/COSOperator.cpp" "${IRRSRC}/CPakReader.cpp" "${IRRSRC}/CReadFile.cpp" "${IRRSRC}/CWriteFile.cpp" "${IRRSRC}/CXMLReader.cpp" "${IRRSRC}/CXMLWriter.cpp" "${IRRSRC}/CZBuffer.cpp" "${IRRSRC}/CZipReader.cpp" "${IRRSRC}/CProtonReader.cpp"
"${IRRSRC}/CImage.cpp" "${IRRSRC}/CImageLoaderBMP.cpp" "${IRRSRC}/CImageLoaderJPG.cpp" "${IRRSRC}/CImageLoaderPNG.cpp" "${IRRSRC}/CImageLoaderRGB.cpp" "${IRRSRC}/CImageLoaderRTTEX.cpp" "${IRRSRC}/CImageLoaderTGA.cpp"
"${IRRMESH}/C3DSMeshFileLoader.cpp" "${IRRMESH}/CAnimatedMeshMD2.cpp" "${IRRMESH}/CAnimatedMeshMD3.cpp" "${IRRMESH}/CB3DMeshFileLoader.cpp" "${IRRMESH}/CBSPMeshFileLoader.cpp" "${IRRMESH}/CColladaFileLoader.cpp" "${IRRMESH}/CCSMLoader.cpp" "${IRRMESH}/CMD2MeshFileLoader.cpp" "${IRRMESH}/CMD3MeshFileLoader.cpp" "${IRRMESH}/CMS3DMeshFileLoader.cpp" "${IRRMESH}/CMY3DMeshFileLoader.cpp" "${IRRMESH}/COBJMeshFileLoader.cpp" "${IRRMESH}/CQ3LevelMesh.cpp" "${IRRMESH}/CQuake3ShaderSceneNode.cpp" "${IRRMESH}/CXMeshFileLoader.cpp"
"${IRRMESH}/CParticleAnimatedMeshSceneNodeEmitter.cpp" "${IRRMESH}/CParticleAttractionAffector.cpp" "${IRRMESH}/CParticleBoxEmitter.cpp" "${IRRMESH}/CParticleCylinderEmitter.cpp" "${IRRMESH}/CParticleFadeOutAffector.cpp" "${IRRMESH}/CParticleGravityAffector.cpp" "${IRRMESH}/CParticleMeshEmitter.cpp" "${IRRMESH}/CParticlePointEmitter.cpp" "${IRRMESH}/CParticleRingEmitter.cpp" "${IRRMESH}/CParticleRotationAffector.cpp" "${IRRMESH}/CParticleScaleAffector.cpp" "${IRRMESH}/CParticleSphereEmitter.cpp" "${IRRMESH}/CParticleSystemSceneNode.cpp"
"${IRRSCENE}/CAnimatedMeshSceneNode.cpp" "${IRRSCENE}/CBillboardSceneNode.cpp" "${IRRSCENE}/CCubeSceneNode.cpp" "${IRRSCENE}/CSceneCollisionManager.cpp" "${IRRSCENE}/CSceneManager.cpp" "${IRRSCENE}/CSceneNodeAnimatorCameraMaya.cpp" "${IRRSCENE}/CSceneNodeAnimatorCollisionResponse.cpp" "${IRRSCENE}/CSceneNodeAnimatorDelete.cpp" "${IRRSCENE}/CSceneNodeAnimatorFlyCircle.cpp" "${IRRSCENE}/CSceneNodeAnimatorFlyStraight.cpp" "${IRRSCENE}/CSceneNodeAnimatorFollowSpline.cpp" "${IRRSCENE}/CSceneNodeAnimatorRotation.cpp" "${IRRSCENE}/CSceneNodeAnimatorTexture.cpp" "${IRRSCENE}/CShadowVolumeSceneNode.cpp"
"${IRRSRC}/CSkyBoxSceneNode.cpp" "${IRRSRC}/CSkyDomeSceneNode.cpp" "${IRRSRC}/CSphereSceneNode.cpp" "${IRRSRC}/CTerrainSceneNode.cpp" "${IRRSRC}/CTerrainTriangleSelector.cpp"
"${IRRSRC}/COpenGLDriver.cpp" "${IRRSRC}/COpenGLExtensionHandler.cpp" "${IRRSRC}/COpenGLNormalMapRenderer.cpp" "${IRRSRC}/COpenGLParallaxMapRenderer.cpp" "${IRRSRC}/COpenGLTexture.cpp" "${IRRSRC}/COpenGLShaderMaterialRenderer.cpp" "${IRRSRC}/COpenGLSLMaterialRenderer.cpp" "${IRRSRC}/CNullDriver.cpp"
"${JPGSRC}/jcapimin.c" "${JPGSRC}/jcapistd.c" "${JPGSRC}/jccoefct.c" "${JPGSRC}/jccolor.c" "${JPGSRC}/jcdctmgr.c" "${JPGSRC}/jchuff.c" "${JPGSRC}/jcinit.c" "${JPGSRC}/jcmainct.c" "${JPGSRC}/jcmarker.c" "${JPGSRC}/jcmaster.c" "${JPGSRC}/jcomapi.c" "${JPGSRC}/jcparam.c" "${JPGSRC}/jcphuff.c" "${JPGSRC}/jcprepct.c" "${JPGSRC}/jcsample.c" "${JPGSRC}/jctrans.c" "${JPGSRC}/jdapimin.c" "${JPGSRC}/jdapistd.c" "${JPGSRC}/jdatadst.c" "${JPGSRC}/jdatasrc.c" "${JPGSRC}/jdcoefct.c" "${JPGSRC}/jdcolor.c" "${JPGSRC}/jddctmgr.c" "${JPGSRC}/jdhuff.c" "${JPGSRC}/jdinput.c" "${JPGSRC}/jdmainct.c" "${JPGSRC}/jdmarker.c" "${JPGSRC}/jdmaster.c" "${JPGSRC}/jdmerge.c" "${JPGSRC}/jdphuff.c" "${JPGSRC}/jdpostct.c" "${JPGSRC}/jdsample.c" "${JPGSRC}/jdtrans.c" "${JPGSRC}/jerror.c" "${JPGSRC}/jfdctflt.c" "${JPGSRC}/jfdctfst.c" "${JPGSRC}/jfdctint.c" "${JPGSRC}/jidctflt.c" "${JPGSRC}/jidctfst.c" "${JPGSRC}/jidctint.c" "${JPGSRC}/jidctred.c" "${JPGSRC}/jmemmgr.c" "${JPGSRC}/jmemnobs.c" "${JPGSRC}/jquant1.c" "${JPGSRC}/jquant2.c" "${JPGSRC}/jutils.c"
"${PNGSRC}/png.c" "${PNGSRC}/pngerror.c" "${PNGSRC}/pnggccrd.c" "${PNGSRC}/pngget.c" "${PNGSRC}/pngmem.c" "${PNGSRC}/pngpread.c" "${PNGSRC}/pngread.c" "${PNGSRC}/pngrio.c" "${PNGSRC}/pngrtran.c" "${PNGSRC}/pngrutil.c" "${PNGSRC}/pngset.c" "${PNGSRC}/pngtrans.c" "${PNGSRC}/pngvcrd.c" "${PNGSRC}/pngwio.c" "${PNGSRC}/pngwtran.c")

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(rt3dapp ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

find_package(SDL REQUIRED)
if(SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
  target_link_libraries(rt3dapp ${SDL_LIBRARY})
endif(SDL_FOUND)

find_package(SDL_mixer REQUIRED)
if(SDLMIXER_FOUND)
  include_directories(${SDLMIXER_INCLUDE_DIR})
  target_link_libraries(rt3dapp ${SDLMIXER_LIBRARY})
else(SDLMIXER_FOUND)
  message(FATAL_ERROR "Couldn't find SDL_mixer. Make sure the SDL_mixer development headers are installed.")
endif(SDLMIXER_FOUND)

find_package(X11 REQUIRED)
if(X11_FOUND)
  include_directories(${X11_INCLUDE_DIR})
  target_link_libraries(rt3dapp ${X11_LIBRARIES})

  if(X11_xf86vmode_FOUND)
    include_directories(${X11_xf86vmode_INCLUDE_PATH})
  endif(X11_xf86vmode_FOUND)
  target_link_libraries(rt3dapp Xxf86vm)
endif(X11_FOUND)

target_link_libraries(rt3dapp rt GL)
