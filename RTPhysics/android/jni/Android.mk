LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := RTPhysics
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
$(SHARED)/PlatformSetup.cpp $(SHARED)/android/AndroidUtils.cpp $(SHARED)/android/AndroidApp.cpp $(SHARED)/Audio/AudioManager.cpp $(SHARED)/Audio/AudioManagerAndroid.cpp\
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
$(COMPPATH)/FilterComponent.cpp\
\
\
$(SHARED)/Irrlicht/IrrlichtManager.cpp $(IRRSRC)/CAttributes.cpp $(IRRSRC)/CBoneSceneNode.cpp $(IRRSRC)/CColorConverter.cpp \
$(IRRSRC)/CDefaultSceneNodeAnimatorFactory.cpp $(IRRSRC)/CDefaultSceneNodeFactory.cpp $(IRRSRC)/CDepthBuffer.cpp $(IRRSRC)/CDummyTransformationSceneNode.cpp $(IRRSRC)/CEmptySceneNode.cpp \
$(IRRSRC)/CFPSCounter.cpp $(IRRSRC)/CGeometryCreator.cpp $(IRRSRC)/CLightSceneNode.cpp $(IRRSRC)/CLogger.cpp $(IRRSRC)/CMemoryFile.cpp \
$(IRRSRC)/CMeshCache.cpp $(IRRSRC)/CMeshManipulator.cpp $(IRRSRC)/CMeshSceneNode.cpp $(IRRSRC)/COCTLoader.cpp $(IRRSRC)/COctreeSceneNode.cpp \
$(IRRSRC)/CSkinnedMesh.cpp $(IRRSRC)/CTextSceneNode.cpp $(IRRSRC)/CTriangleBBSelector.cpp $(IRRSRC)/CTriangleSelector.cpp $(IRRSRC)/COctreeTriangleSelector.cpp \
$(IRRSRC)/CVideoModeList.cpp $(IRRSRC)/CVolumeLightSceneNode.cpp $(IRRSRC)/CWaterSurfaceSceneNode.cpp $(IRRSRC)/Irrlicht.cpp \
$(IRRSRC)/irrXML.cpp $(IRRSRC)/os.cpp $(IRRSRC)/CMetaTriangleSelector.cpp \
\
$(IRRSRC)/CCameraSceneNode.cpp $(IRRSRC)/CSceneNodeAnimatorCameraFPS.cpp $(IRRSRC)/CIrrDeviceIPhone.cpp $(IRRSRC)/CIrrDeviceStub.cpp \
\
$(IRRSRC)/CFileList.cpp $(IRRSRC)/CFileSystem.cpp $(IRRSRC)/CLimitReadFile.cpp $(IRRSRC)/CMountPointReader.cpp \
$(IRRSRC)/COSOperator.cpp $(IRRSRC)/CPakReader.cpp $(IRRSRC)/CReadFile.cpp $(IRRSRC)/CWriteFile.cpp  $(IRRSRC)/CXMLReader.cpp  $(IRRSRC)/CXMLWriter.cpp \
$(IRRSRC)/CZBuffer.cpp $(IRRSRC)/CZipReader.cpp $(IRRSRC)/CProtonReader.cpp \
\
$(IRRSRC)/CImage.cpp $(IRRSRC)/CImageLoaderBMP.cpp $(IRRSRC)/CImageLoaderJPG.cpp $(IRRSRC)/CImageLoaderPNG.cpp  $(IRRSRC)/CImageLoaderRGB.cpp  $(IRRSRC)/CImageLoaderRTTEX.cpp $(IRRSRC)/CImageLoaderTGA.cpp \
\
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
    $(BULLET)/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btDbvt.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
	$(BULLET)/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btGhostObject.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/btUnionFind.cpp \
	$(BULLET)/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btBoxShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btBox2dShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btCollisionShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btCompoundShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConcaveShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConeShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConvexShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btCylinderShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btEmptyShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btShapeHull.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btSphereShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
	$(BULLET)/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
	$(BULLET)/BulletCollision/Gimpact/btContactProcessing.cpp \
	$(BULLET)/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
	$(BULLET)/BulletCollision/Gimpact/btGImpactBvh.cpp \
	$(BULLET)/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
	$(BULLET)/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
	$(BULLET)/BulletCollision/Gimpact/btGImpactShape.cpp \
	$(BULLET)/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
	$(BULLET)/BulletCollision/Gimpact/gim_box_set.cpp \
	$(BULLET)/BulletCollision/Gimpact/gim_contact.cpp \
	$(BULLET)/BulletCollision/Gimpact/gim_memory.cpp \
	$(BULLET)/BulletCollision/Gimpact/gim_tri_collision.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
	$(BULLET)/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
	$(BULLET)/BulletDynamics/Character/btKinematicCharacterController.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
	$(BULLET)/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
	$(BULLET)/BulletDynamics/Dynamics/btContinuousDynamicsWorld.cpp \
	$(BULLET)/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
	$(BULLET)/BulletDynamics/Dynamics/btRigidBody.cpp \
	$(BULLET)/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
	$(BULLET)/BulletDynamics/Dynamics/Bullet-C-API.cpp \
	$(BULLET)/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
	$(BULLET)/BulletDynamics/Vehicle/btWheelInfo.cpp \
	$(BULLET)/LinearMath/btAlignedAllocator.cpp \
	$(BULLET)/LinearMath/btConvexHull.cpp \
	$(BULLET)/LinearMath/btGeometryUtil.cpp \
	$(BULLET)/LinearMath/btQuickprof.cpp \
	$(BULLET)/LinearMath/btSerializer.cpp \
        $(BULLET)/BulletSoftBody/btDefaultSoftBodySolver.cpp \
        $(BULLET)/BulletSoftBody/btSoftBody.cpp \
        $(BULLET)/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp \
        $(BULLET)/BulletSoftBody/btSoftBodyHelpers.cpp \
        $(BULLET)/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp \
        $(BULLET)/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp \
        $(BULLET)/BulletSoftBody/btSoftRigidDynamicsWorld.cpp \
        $(BULLET)/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp \
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
\
$(APP)/App.cpp $(APP)/GUI/DebugMenu.cpp $(APP)/GUI/MainMenu.cpp $(APP)/GUI/PhysicsHelloMenu.cpp \
\
$(APPCOMP)/FPSControlComponent.cpp



LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_SHARED_LIBRARY)
