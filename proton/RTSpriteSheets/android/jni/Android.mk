LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := rtspritesheets
SHARED := ../../../shared
APP := ../../source
LOCAL_ARM_MODE := arm

COMPPATH := ../../../shared/Entity
CLANMATH := $(SHARED)/ClanLib-2.0/Sources/Core/Math

LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

#release flags
#SHARED_FLAGS := -DANDROID_NDK -DBUILD_ANDROID -DRT_SPRITEANIMATION -DNDEBUG

#debug flags
SHARED_FLAGS := -DANDROID_NDK -DBUILD_ANDROID -DRT_SPRITEANIMATION -D_DEBUG

LOCAL_CFLAGS := -DGC_BUILD_ANDROID $(SHARED_FLAGS)
LOCAL_CPPFLAGS := -DGC_BUILD_C $(SHARED_FLAGS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SHARED) \
 $(LOCAL_PATH)/$(APP) $(LOCAL_PATH)/$(SHARED)/ClanLib-2.0/Sources $(LOCAL_PATH)/$(SHARED)/util/boost
                
LOCAL_SRC_FILES := \
	$(SHARED)/PlatformSetup.cpp $(SHARED)/android/AndroidUtils.cpp $(SHARED)/android/AndroidApp.cpp $(SHARED)/Audio/AudioManager.cpp $(SHARED)/Audio/AudioManagerAndroid.cpp \
$(CLANMATH)/angle.cpp $(CLANMATH)/mat3.cpp $(CLANMATH)/mat4.cpp $(CLANMATH)/rect.cpp $(CLANMATH)/vec2.cpp $(CLANMATH)/vec3.cpp $(CLANMATH)/vec4.cpp \
$(SHARED)/Entity/Entity.cpp $(SHARED)/Entity/Component.cpp $(SHARED)/GUI/RTFont.cpp $(SHARED)/Manager/Console.cpp \
$(SHARED)/Manager/GameTimer.cpp $(SHARED)/Manager/MessageManager.cpp $(SHARED)/Manager/ResourceManager.cpp $(SHARED)/Manager/VariantDB.cpp $(SHARED)/Math/rtPlane.cpp \
$(SHARED)/Math/rtRect.cpp $(SHARED)/Renderer/RenderBatcher.cpp $(SHARED)/Renderer/SoftSurface.cpp $(SHARED)/Renderer/Surface.cpp $(SHARED)/Renderer/SurfaceAnim.cpp \
$(SHARED)/Renderer/SpriteSheetSurface.cpp $(SHARED)/Renderer/SpriteAnimation.cpp \
$(SHARED)/util/CRandom.cpp $(SHARED)/util/GLESUtils.cpp $(SHARED)/util/MathUtils.cpp $(SHARED)/util/MiscUtils.cpp $(SHARED)/util/RenderUtils.cpp $(SHARED)/util/ResourceUtils.cpp \
$(SHARED)/util/Variant.cpp $(SHARED)/util/boost/libs/signals/src/connection.cpp $(SHARED)/util/boost/libs/signals/src/named_slot_map.cpp $(SHARED)/util/boost/libs/signals/src/signal_base.cpp \
$(SHARED)/util/boost/libs/signals/src/slot.cpp $(SHARED)/util/boost/libs/signals/src/trackable.cpp $(SHARED)/BaseApp.cpp \
$(SHARED)/util/unzip/unzip.c $(SHARED)/util/unzip/ioapi.c $(SHARED)/util/TextScanner.cpp $(SHARED)/Entity/EntityUtils.cpp $(SHARED)/Entity/SpriteAnimationUtils.cpp \
$(SHARED)/FileSystem/StreamingInstance.cpp \
$(SHARED)/FileSystem/StreamingInstanceZip.cpp $(SHARED)/FileSystem/StreamingInstanceFile.cpp $(SHARED)/FileSystem/FileSystem.cpp $(SHARED)/FileSystem/FileSystemZip.cpp \
$(SHARED)/FileSystem/FileManager.cpp \
$(SHARED)/testfw/ProtonTester.cpp $(SHARED)/testfw/ProtonTesterGUI.cpp \
\
$(COMPPATH)/Button2DComponent.cpp $(COMPPATH)/FilterInputComponent.cpp $(COMPPATH)/FocusInputComponent.cpp $(COMPPATH)/FocusRenderComponent.cpp $(COMPPATH)/FocusUpdateComponent.cpp \
$(COMPPATH)/InputTextRenderComponent.cpp $(COMPPATH)/InterpolateComponent.cpp $(COMPPATH)/OverlayRenderComponent.cpp $(COMPPATH)/ProgressBarComponent.cpp \
$(COMPPATH)/RectRenderComponent.cpp $(COMPPATH)/ScrollBarRenderComponent.cpp $(COMPPATH)/ScrollComponent.cpp $(COMPPATH)/TapSequenceDetectComponent.cpp $(COMPPATH)/TextBoxRenderComponent.cpp \
$(COMPPATH)/TextRenderComponent.cpp $(COMPPATH)/TouchStripComponent.cpp $(COMPPATH)/TrailRenderComponent.cpp $(COMPPATH)/TyperComponent.cpp $(COMPPATH)/UnderlineRenderComponent.cpp \
$(COMPPATH)/TouchHandlerComponent.cpp $(COMPPATH)/SelectButtonWithCustomInputComponent.cpp $(COMPPATH)/CustomInputComponent.cpp $(COMPPATH)/SliderComponent.cpp $(COMPPATH)/EmitVirtualKeyComponent.cpp \
$(COMPPATH)/RenderScissorComponent.cpp $(COMPPATH)/SpriteAnimationRenderComponent.cpp \
\
$(APP)/App.cpp $(APP)/GUI/GUICommon.cpp $(APP)/GUI/MainMenu.cpp $(APP)/GUI/BasicSpriteFramesScreen.cpp \ $(APP)/GUI/SpriteAnimationScreen.cpp $(APP)/GUI/TestScreen.cpp


LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_SHARED_LIBRARY)
