#include <PlatformSetup.h>
#include <jni.h>
#include <App.h>

extern "C" 
{

	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_Main_nativeOnKey(  JNIEnv*  env, jobject thiz,jint type, jint keycode, jint c )
	{
		AppOnKey(env, thiz, type, keycode, c);
	}
	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_Main_nativeOnAccelerometerUpdate(  JNIEnv*  env, jobject thiz, jfloat x, jfloat y, jfloat z)
	{
		AppOnAccelerometerUpdate(env, thiz, x, y, z);
	}
	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppGLSurfaceView_nativePause( JNIEnv*  env )
	{
		AppPause(env);
	}
	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppGLSurfaceView_nativeResume( JNIEnv*  env )
	{
		AppResume(env);
	}
	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppGLSurfaceView_nativeOnTouch(  JNIEnv*  env, jobject thiz, jint action, jfloat x, jfloat y, jint finger )
	{
		AppOnTouch(env, thiz, action, x, y, finger);
	}

	JNIEXPORT int Java_com_rtsoft_rtbarebones_AppRenderer_nativeOSMessageGet(JNIEnv*  env)
	{
		return AppOSMessageGet(env); 
	}
	JNIEXPORT float Java_com_rtsoft_rtbarebones_AppRenderer_nativeGetLastOSMessageX(JNIEnv*  env)
	{
		return AppGetLastOSMessageX(env); 
	}

	/* Call to initialize the graphics state */
	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppRenderer_nativeInit( JNIEnv*  env )
	{
		AppInit(env);
	}

	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppRenderer_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
	{
		AppResize(env, thiz, w, h);
	}

	/* Call to finalize the graphics state */
	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppRenderer_nativeDone( JNIEnv*  env )
	{
		AppDone(env);
	}

	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppRenderer_nativeUpdate( JNIEnv*  env )
	{
		AppUpdate(env);
	}
	JNIEXPORT void JNICALL Java_com_rtsoft_rtbarebones_AppRenderer_nativeRender( JNIEnv*  env )
	{
		AppRender(env);
	}

}