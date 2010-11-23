#pragma once
#include <windows.h>
#include <TCHAR.h>

#ifdef C_GL_MODE
#include "GL/gl.h"
#include "glext.h" //needed for GL_CLAMP_TO_EDGE

//help with compatibility so I can use the GL ES calls with normal GL

#define glTexParameterx glTexParameteri
#define glFrustumf glFrustum
#define glOrthof glOrtho
#define glColor4x(r,g,b,a) glColor4f( (float(r)/65536.0f),  (float(g)/65536.0f) , (float(b)/65536.0f), (float(a)/65536.0f));

#else

#ifdef RT_WEBOS
#include <GLES/gl.h>
#else
#include <GLES/egl.h>
#include <GLES/gl.h>
#endif

#ifdef _IRR_STATIC_LIB_
	#include "Irrlicht/source/Irrlicht/gles-ext.h"
#endif

typedef GLfloat GLdouble;
#define glClipPlane glClipPlanef
#endif


#define M_PI 3.141592f

/* The following definitions are the same across platforms.  This first
** group are the sanctioned types.
*/
#ifndef _BOOLEAN_DEFINED
typedef  unsigned char      boolean;     /* Boolean value type. */
#define _BOOLEAN_DEFINED
#endif

#ifndef _UINT32_DEFINED
typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
#define _UINT32_DEFINED
#endif

#ifndef _UINT16_DEFINED
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
#define _UINT16_DEFINED
#endif

#ifndef _UINT8_DEFINED
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
#define _UINT8_DEFINED
#endif

#ifndef _INT32_DEFINED
typedef  signed long int    int32;       /* Signed 32 bit value */
#define _INT32_DEFINED
#endif

#ifndef _INT16_DEFINED
typedef  signed short       int16;       /* Signed 16 bit value */
#define _INT16_DEFINED
#endif

#ifndef _INT8_DEFINED
typedef  signed char        int8;        /* Signed 8  bit value */
#define _INT8_DEFINED
#endif

#if defined(__GNUC__)
#define __int64 long long
#endif

#ifndef _UINT64_DEFINED
typedef  unsigned __int64   uint64;      /* Unsigned 64 bit value */
#define _UINT64_DEFINED
#endif

#ifndef _INT64_DEFINED
typedef  __int64            int64;       /* Signed 64 bit value */
#define _INT64_DEFINED
#endif

#ifndef _SINT64_DEFINED
typedef  __int64            sint64;       /* Signed 64 bit value */
#define _SINT64_DEFINED
#endif

#ifndef _BYTE_DEFINED
typedef  unsigned char      byte;        /* byte type */
#define  _BYTE_DEFINED
#endif


#include "WinUtils.h"
