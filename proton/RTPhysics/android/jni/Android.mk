LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := rtphysics
SHARED := ../../../shared
APP := ../../source
APPCOMP := ../../source/Component
LOCAL_ARM_MODE := arm

COMPPATH := ../../../shared/Entity
CLANMATH := $(SHARED)/ClanLib-2.0/Sources/Core/Math
ZLIBPATH := $(SHARED)/util/zlib
IRRSRC :=  $(SHARED)/Irrlicht/source/Irrlicht
IRRMESH := $(IRRSRC)
IRRPARTICLE := $(IRRSRC)
IRRSCENE := $(IRRSRC)
PNGSRC :=  $(SHARED)/Irrlicht/source/Irrlicht/libpng
JPGSRC :=  $(SHARED)/Irrlicht/source/Irrlicht/jpeglib
LZMASRC :=  $(SHARED)/Irrlicht/source/Irrlicht/lzma
PPATH := ../../../shared/Renderer/linearparticle/sources
BULLET := $(SHARED)/Bullet
IRRBULLET := $(SHARED)/Irrlicht/irrBullet

#LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

#release flags
LOCAL_CFLAGS := -DANDROID_NDK -DBUILD_ANDROID -DGC_BUILD_ANDROID -DNDEBUG -D_IRR_STATIC_LIB_ -DRT_IRRBULLET
LOCAL_CPPFLAGS := -DGC_BUILD_C -DANDROID_NDK -DBUILD_ANDROID -DNDEBUG -D_IRR_STATIC_LIB_ -DRT_IRRBULLET

#debug flags
#LOCAL_CFLAGS := -DANDROID_NDK -DBUILD_ANDROID -DGC_BUILD_ANDROID -D_DEBUG -D_IRR_STATIC_LIB_ -DRT_IRRBULLET
#LOCAL_CPPFLAGS := -DGC_BUILD_C -DANDROID_NDK -DBUILD_ANDROID -D_DEBUG -D_IRR_STATIC_LIB_ -DRT_IRRBULLET

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SHARED) $(LOCAL_PATH)/$(SHARED)/Irrlicht/include \
 $(LOCAL_PATH)/$(APP) $(LOCAL_PATH)/$(SHARED)/ClanLib-2.0/Sources $(LOCAL_PATH)/$(SHARED)/util/boost \
      $(LOCAL_PATH)/$(BULLET)
                

LOCAL_SRC_FILES := \
$(SHARED)/PlatformSetup.cpp $(SHARED)/android/AndroidUtils.cpp ../temp_final_cpp_src/AndroidApp.cpp $(SHARED)/Audio/AudioManager.cpp $(SHARED)/Audio/AudioManagerAndroid.cpp\
$(CLANMATH)/angle.cpp $(CLANMATH)/mat3.cpp $(CLANMATH)/mat4.cpp $(CLANMATH)/rect.cpp $(CLANMATH)/vec2.cpp $(CLANMATH)/vec3.cpp $(CLANMATH)/vec4.cpp \
$(SHARED)/Entity/Entity.cpp $(SHARED)/Entity/Component.cpp $(SHARED)/GUI/RTFont.cpp $(SHARED)/Manager/Console.cpp \
$(SHARED)/Manager/GameTimer.cpp $(SHARED)/Manager/MessageManager.cpp $(SHARED)/Manager/ResourceManager.cpp $(SHARED)/Manager/VariantDB.cpp $(SHARED)/Math/rtPlane.cpp \
$(SHARED)/Math/rtRect.cpp $(SHARED)/Renderer/RenderBatcher.cpp $(SHARED)/Renderer/SoftSurface.cpp $(SHARED)/Renderer/Surface.cpp $(SHARED)/Renderer/SurfaceAnim.cpp \
$(SHARED)/util/CRandom.cpp $(SHARED)/util/GLESUtils.cpp $(SHARED)/util/MathUtils.cpp $(SHARED)/util/MiscUtils.cpp $(SHARED)/util/RenderUtils.cpp $(SHARED)/util/ResourceUtils.cpp \
$(SHARED)/util/Variant.cpp $(SHARED)/util/boost/libs/signals/src/connection.cpp $(SHARED)/util/boost/libs/signals/src/named_slot_map.cpp $(SHARED)/util/boost/libs/signals/src/signal_base.cpp \
$(SHARED)/util/boost/libs/signals/src/slot.cpp $(SHARED)/util/boost/libs/signals/src/trackable.cpp $(SHARED)/BaseApp.cpp \
$(SHARED)/util/unzip/unzip.c $(SHARED)/util/unzip/ioapi.c $(SHARED)/util/TextScanner.cpp $(SHARED)/Entity/EntityUtils.cpp \
$(SHARED)/Network/NetHTTP.cpp $(SHARED)/Network/NetSocket.cpp $(SHARED)/Network/NetUtils.cpp $(SHARED)/Audio/AudioManagerSDL.cpp  $(SHARED)/FileSystem/StreamingInstance.cpp \
$(SHARED)/FileSystem/StreamingInstanceZip.cpp $(SHARED)/FileSystem/StreamingInstanceFile.cpp $(SHARED)/FileSystem/FileSystem.cpp $(SHARED)/FileSystem/FileSystemZip.cpp \
$(SHARED)/FileSystem/FileManager.cpp \
\
\
$(COMPPATH)/Button2DComponent.cpp $(COMPPATH)/FilterInputComponent.cpp $(COMPPATH)/FocusInputComponent.cpp $(COMPPATH)/FocusRenderComponent.cpp $(COMPPATH)/FocusUpdateComponent.cpp \
$(COMPPATH)/HTTPComponent.cpp $(COMPPATH)/InputTextRenderComponent.cpp $(COMPPATH)/InterpolateComponent.cpp $(COMPPATH)/OverlayRenderComponent.cpp $(COMPPATH)/ProgressBarComponent.cpp \
$(COMPPATH)/RectRenderComponent.cpp $(COMPPATH)/ScrollBarRenderComponent.cpp $(COMPPATH)/ScrollComponent.cpp $(COMPPATH)/TapSequenceDetectComponent.cpp $(COMPPATH)/TextBoxRenderComponent.cpp \
$(COMPPATH)/TextRenderComponent.cpp $(COMPPATH)/TouchStripComponent.cpp $(COMPPATH)/TrailRenderComponent.cpp $(COMPPATH)/TyperComponent.cpp $(COMPPATH)/UnderlineRenderComponent.cpp \
$(COMPPATH)/TouchHandlerComponent.cpp $(COMPPATH)/SelectButtonWithCustomInputComponent.cpp $(COMPPATH)/CustomInputComponent.cpp $(COMPPATH)/SliderComponent.cpp $(COMPPATH)/EmitVirtualKeyComponent.cpp\
$(COMPPATH)/FilterComponent.cpp $(COMPPATH)/RenderScissorComponent.cpp \
\
\
$(SHARED)/Irrlicht/IrrlichtManager.cpp $(IRRSRC)/UnityBuild_IrrlichtMain.cpp \
$(JPGSRC)/jcapimin.c $(JPGSRC)/jcapistd.c $(JPGSRC)/jccoefct.c $(JPGSRC)/jccolor.c $(JPGSRC)/jcdctmgr.c $(JPGSRC)/jchuff.c $(JPGSRC)/jcinit.c $(JPGSRC)/jcmainct.c \
$(JPGSRC)/jcmarker.c $(JPGSRC)/jcmaster.c $(JPGSRC)/jcomapi.c $(JPGSRC)/jcparam.c $(JPGSRC)/jcphuff.c $(JPGSRC)/jcprepct.c $(JPGSRC)/jcsample.c $(JPGSRC)/jctrans.c \
$(JPGSRC)/jdapimin.c $(JPGSRC)/jdapistd.c $(JPGSRC)/jdatadst.c $(JPGSRC)/jdatasrc.c $(JPGSRC)/jdcoefct.c $(JPGSRC)/jdcolor.c $(JPGSRC)/jddctmgr.c \
$(JPGSRC)/jdhuff.c $(JPGSRC)/jdinput.c $(JPGSRC)/jdmainct.c $(JPGSRC)/jdmarker.c $(JPGSRC)/jdmaster.c $(JPGSRC)/jdmerge.c $(JPGSRC)/jdphuff.c $(JPGSRC)/jdpostct.c \
$(JPGSRC)/jdsample.c $(JPGSRC)/jdtrans.c $(JPGSRC)/jerror.c $(JPGSRC)/jfdctflt.c $(JPGSRC)/jfdctfst.c $(JPGSRC)/jfdctint.c $(JPGSRC)/jidctflt.c $(JPGSRC)/jidctfst.c \
$(JPGSRC)/jidctint.c $(JPGSRC)/jidctred.c $(JPGSRC)/jmemmgr.c $(JPGSRC)/jmemnobs.c $(JPGSRC)/jquant1.c $(JPGSRC)/jquant2.c $(JPGSRC)/jutils.c \
\
$(PNGSRC)/png.c $(PNGSRC)/pngerror.c $(PNGSRC)/pnggccrd.c $(PNGSRC)/pngget.c $(PNGSRC)/pngmem.c $(PNGSRC)/pngpread.c $(PNGSRC)/pngread.c \
$(PNGSRC)/pngrio.c $(PNGSRC)/pngrtran.c $(PNGSRC)/pngrutil.c $(PNGSRC)/pngset.c $(PNGSRC)/pngtrans.c $(PNGSRC)/pngvcrd.c $(PNGSRC)/pngwio.c $(PNGSRC)/pngwtran.c \
\
$(IRRMESH)/C3DSMeshFileLoader.cpp $(IRRMESH)/CAnimatedMeshMD2.cpp $(IRRMESH)/CAnimatedMeshMD3.cpp $(IRRMESH)/CB3DMeshFileLoader.cpp $(IRRMESH)/CBSPMeshFileLoader.cpp \
$(IRRMESH)/CColladaFileLoader.cpp $(IRRMESH)/CCSMLoader.cpp $(IRRMESH)/CMD2MeshFileLoader.cpp $(IRRMESH)/CMD3MeshFileLoader.cpp $(IRRMESH)/CMS3DMeshFileLoader.cpp \
$(IRRMESH)/CMY3DMeshFileLoader.cpp $(IRRMESH)/COBJMeshFileLoader.cpp $(IRRMESH)/CQ3LevelMesh.cpp $(IRRMESH)/CQuake3ShaderSceneNode.cpp $(IRRMESH)/CXMeshFileLoader.cpp \
\
$(IRRMESH)/CParticleAnimatedMeshSceneNodeEmitter.cpp $(IRRMESH)/CParticleAttractionAffector.cpp $(IRRMESH)/CParticleBoxEmitter.cpp $(IRRMESH)/CParticleCylinderEmitter.cpp $(IRRMESH)/CParticleFadeOutAffector.cpp \
$(IRRMESH)/CParticleGravityAffector.cpp $(IRRMESH)/CParticleMeshEmitter.cpp $(IRRMESH)/CParticlePointEmitter.cpp $(IRRMESH)/CParticleRingEmitter.cpp $(IRRMESH)/CParticleRotationAffector.cpp \
$(IRRMESH)/CParticleScaleAffector.cpp $(IRRMESH)/CParticleSphereEmitter.cpp $(IRRMESH)/CParticleSystemSceneNode.cpp \
\
$(IRRSCENE)/CAnimatedMeshSceneNode.cpp $(IRRSCENE)/CBillboardSceneNode.cpp $(IRRSCENE)/CCubeSceneNode.cpp $(IRRSCENE)/CSceneCollisionManager.cpp $(IRRSCENE)/CSceneManager.cpp \
$(IRRSCENE)/CSceneNodeAnimatorCameraMaya.cpp $(IRRSCENE)/CSceneNodeAnimatorCollisionResponse.cpp $(IRRSCENE)/CSceneNodeAnimatorDelete.cpp $(IRRSCENE)/CSceneNodeAnimatorFlyCircle.cpp $(IRRSCENE)/CSceneNodeAnimatorFlyStraight.cpp \
$(IRRSCENE)/CSceneNodeAnimatorFollowSpline.cpp $(IRRSCENE)/CSceneNodeAnimatorRotation.cpp $(IRRSCENE)/CSceneNodeAnimatorTexture.cpp $(IRRSCENE)/CShadowVolumeSceneNode.cpp \
\
$(IRRSRC)/CSkyBoxSceneNode.cpp $(IRRSRC)/CSkyDomeSceneNode.cpp $(IRRSRC)/CSphereSceneNode.cpp $(IRRSRC)/CTerrainSceneNode.cpp $(IRRSRC)/CTerrainTriangleSelector.cpp \
\
$(IRRSRC)/COGLESDriver.cpp $(IRRSRC)/COGLESExtensionHandler.cpp $(IRRSRC)/COGLESTexture.cpp $(IRRSRC)/CNullDriver.cpp \
\
\
$(BULLET)/UnityBuild_Bullet.cpp \
$(BULLET)/UnityBuild_BulletGImpact.cpp \
$(BULLET)/BulletCollision/Gimpact/gim_contact.cpp \
\
\
$(IRRBULLET)/boxshape.cpp \
$(IRRBULLET)/bulletworld.cpp \
$(IRRBULLET)/bvhtrianglemeshshape.cpp \
$(IRRBULLET)/collisioncallbackinformation.cpp \
$(IRRBULLET)/collisionobject.cpp \
$(IRRBULLET)/collisionobjectaffector.cpp \
$(IRRBULLET)/collisionobjectaffectorattract.cpp \
$(IRRBULLET)/collisionobjectaffectordelete.cpp \
$(IRRBULLET)/collisionshape.cpp \
$(IRRBULLET)/convexhullshape.cpp \
$(IRRBULLET)/gimpactmeshshape.cpp \
$(IRRBULLET)/irrbullet.cpp \
$(IRRBULLET)/irrbulletcommon.cpp \
$(IRRBULLET)/liquidbody.cpp \
$(IRRBULLET)/motionstate.cpp \
$(IRRBULLET)/physicsdebug.cpp \
$(IRRBULLET)/raycastvehicle.cpp \
$(IRRBULLET)/rigidbody.cpp \
$(IRRBULLET)/softbody.cpp \
$(IRRBULLET)/sphereshape.cpp \
$(IRRBULLET)/trianglemeshshape.cpp \
\
$(APP)/App.cpp $(APP)/GUI/DebugMenu.cpp $(APP)/GUI/MainMenu.cpp $(APP)/GUI/PhysicsHelloMenu.cpp \
\
$(APPCOMP)/FPSControlComponent.cpp



LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_SHARED_LIBRARY)
