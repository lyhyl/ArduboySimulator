//  ***************************************************************
//  WinUtils - Creation date: 09/12/2010
//  -------------------------------------------------------------
//  Robinson Technologies - Check license.txt for license info.
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef WebOSUtils_h__
#define WebOSUtils_h__
#include "../PlatformEnums.h"
#include <jni.h>
std::string GetAPKFile();

JNIEnv * GetJavaEnv();
char * GetAndroidMainClassName();
void AppResize( JNIEnv*  env, jobject  thiz, jint w, jint h );
void AppUpdate(JNIEnv*  env);
void AppRender(JNIEnv*  env);
void AppDone(JNIEnv* env);
void AppPause(JNIEnv* env);
void AppInit(JNIEnv* env);
void AppResume(JNIEnv* env);
void AppOnTouch( JNIEnv* env, jobject jobj,jint action, jfloat x, jfloat y, jint finger);
void AppOnKey( JNIEnv* env, jobject jobj, jint type, jint keycode, jint c);
int AppOSMessageGet(JNIEnv* env);
float AppGetLastOSMessageX(JNIEnv* env);
void AppOnAccelerometerUpdate(JNIEnv* env, jobject jobj, jfloat x, jfloat y, jfloat z);
void AppOnTrackball(JNIEnv* env, jobject jobj, jfloat x, jfloat y);
void AppOnSendGUIEx(JNIEnv*  env, jobject thiz,jint messageType, jint parm1, jint parm2, jint finger );
jstring AppGetLastOSMessageString(JNIEnv* env);
void AppOnSendGUIStringEx(JNIEnv* env, jobject thiz,jint messageType, jint parm1, jint parm2, jint finger, jstring s );
float AppGetLastOSMessageParm1(JNIEnv* env);
#endif // WebOSUtils_h__
