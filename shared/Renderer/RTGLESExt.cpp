#include "PlatformPrecomp.h"
#include "RTGLESExt.h"

//you must call InitExtensions on this before using though.  I don't want it in the constructor because I want to be able to control
//the order of initialization

RTGLESExt g_glesExt;

RTGLESExt::RTGLESExt()
{
}

RTGLESExt::~RTGLESExt()
{
}

void RTGLESExt::InitExtensions()
{
	
/*
	pGlDrawTexiOES = (PFNGLDRAWTEXIOES) eglGetProcAddress("glDrawTexiOES");
	pGlDrawTexfOES = (PFNGLDRAWTEXFOES) eglGetProcAddress("glDrawTexfOES");
	pGlDrawTexivOES = (PFNGLDRAWTEXIVOES) eglGetProcAddress("glDrawTexivOES");
	pGlDrawTexfvOES = (PFNGLDRAWTEXFVOES) eglGetProcAddress("glDrawTexfvOES");

	pGlBindRenderbufferOES = (PFNGLBINDRENDERBUFFEROES) eglGetProcAddress("glBindRenderbufferOES");
	pGlDeleteRenderbuffersOES = (PFNGLDELETERENDERBUFFERSOES) eglGetProcAddress("glDeletedRenderbuffersOES");
	pGlGenRenderbuffersOES = (PFNGLGENRENDERBUFFERSOES) eglGetProcAddress("glGenRenderbuffersOES");
	pGlRenderbufferStorageOES = (PFNGLRENDERBUFFERSTORAGEOES) eglGetProcAddress("glRenderbufferStorageOES");
	pGlBindFramebufferOES = (PFNGLBINDFRAMEBUFFEROES) eglGetProcAddress("glBindFramebufferOES");
	pGlDeleteFramebuffersOES = (PFNGLDELETEFRAMEBUFFERSOES) eglGetProcAddress("glDeleteFramebuffersOES");
	pGlGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOES) eglGetProcAddress("glGenFramebuffersOES");
	pGlCheckFramebufferStatusOES = (PFNGLCHECKFRAMEBUFFERSTATUSOES) eglGetProcAddress("glCheckFramebufferStatusOES");
	pGlFramebufferRenderbufferOES = (PFNGLFRAMEBUFFERRENDERBUFFEROES) eglGetProcAddress("glFramebufferRenderbufferOES");
	pGlFramebufferTexture2DOES = (PFNGLFRAMEBUFFERTEXTURE2DOES) eglGetProcAddress("glFramebufferTexture2DOES");
	pGlGenerateMipMapOES = (PFNGLGENERATEMIPMAPOES) eglGetProcAddress("glGenerateMipMapOES");
	*/
}