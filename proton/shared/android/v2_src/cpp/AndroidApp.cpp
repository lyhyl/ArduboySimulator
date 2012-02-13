//** Note, this gets javapp run on it later during the ant build process on Java, which creates a temporary version with
//our class name hardcoded in it

#include <PlatformSetup.h>
#include <jni.h>
#include <BaseApp.h>

extern "C" 
{

	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_SharedActivity_nativeOnKey(  JNIEnv*  env, jobject thiz,jint type, jint keycode, jint c )
	{
		AppOnKey(env, thiz, type, keycode, c);
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_SharedActivity_nativeSendGUIEx(  JNIEnv*  env, jobject thiz,jint messageType, jint parm1, jint parm2, jint finger )
	{
		AppOnSendGUIEx(env, thiz, messageType, parm1, parm2, finger);
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_SharedActivity_nativeSendGUIStringEx(  JNIEnv*  env, jobject thiz,jint messageType, jint parm1, jint parm2, jint finger, jstring s )
	{
		AppOnSendGUIStringEx(env, thiz, messageType, parm1, parm2, finger,s );
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_SharedActivity_nativeOnAccelerometerUpdate(  JNIEnv*  env, jobject thiz, jfloat x, jfloat y, jfloat z)
	{
		AppOnAccelerometerUpdate(env, thiz, x, y, z);
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_SharedActivity_nativeOnTrackball(  JNIEnv*  env, jobject thiz, jfloat x, jfloat y)
	{
		AppOnTrackball(env, thiz, x, y);
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppGLSurfaceView_nativePause( JNIEnv*  env )
	{
		AppPause(env);
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppGLSurfaceView_nativeResume( JNIEnv*  env )
	{
		AppResume(env);
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppGLSurfaceView_nativeOnTouch(  JNIEnv*  env, jobject thiz, jint action, jfloat x, jfloat y, jint finger )
	{
		AppOnTouch(env, thiz, action, x, y, finger);
	}
 
	JNIEXPORT int Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeOSMessageGet(JNIEnv*  env)
	{
		return AppOSMessageGet(env); 
	}
	JNIEXPORT float Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeGetLastOSMessageX(JNIEnv*  env)
	{
		return AppGetLastOSMessageX(env); 
	}

	JNIEXPORT int Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeGetLastOSMessageParm1(JNIEnv*  env)
	{
		return AppGetLastOSMessageParm1(env); 
	}



	JNIEXPORT jstring Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeGetLastOSMessageString(JNIEnv*  env)
	{
		return AppGetLastOSMessageString(env);
	}

	/* Call to initialize the graphics state */
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeInit( JNIEnv*  env )
	{
		AppInit(env);
	}

	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
	{
		AppResize(env, thiz, w, h);
	}

	/* Call to finalize the graphics state */
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeDone( JNIEnv*  env )
	{
		AppDone(env);
	}

	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeUpdate( JNIEnv*  env )
	{
		AppUpdate(env);
	}
	JNIEXPORT void JNICALL Java_${PACKAGE_NAME_WITH_UNDERSCORES}_AppRenderer_nativeRender( JNIEnv*  env )
	{
		AppRender(env);
	}

}