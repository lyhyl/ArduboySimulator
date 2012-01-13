/*
 *  RenderUtils.h
 *  
 *
 *  Created by Seth Robinson on 3/18/09.
 *  Copyright 2009 Robinson Technologies. All rights reserved.
 *
 */

#ifndef RenderUtils_h__
#define RenderUtils_h__
#include "Math/rtRect.h"
#include "util/MathUtils.h"

class Surface;
class Entity;
#ifdef _DEBUG
#define CALL_THE_QGL_VERSION_OF_glGetError glGetError
#define CHECK_GL_ERROR() { GLenum __error = glGetError(); if(__error) {LogError("OpenGL error 0x%04X (%d) in %s line %d\n", __error,__error, __FUNCTION__, __LINE__); /*assert(0);*/ } }
#else
#define CHECK_GL_ERROR() 
#endif


#define C_APP_FOV 45.0f


void RenderGLTriangle();
void RenderTexturedGLTriangle();
void RenderTexturedRectangle(float RectSize=1.0f);
void RenderSpinningTriangle();

int GetScreenSizeY();
int GetScreenSizeX();
float GetScreenSizeYf();
float GetScreenSizeXf();
CL_Vec2f GetScreenSize();

void SetupScreenInfo(int x, int y, int orientation);
bool SetupScreenInfoIPhone(int interfaceOrientation);

void ConvertCoordinatesIfRequired(int &xPos, int &yPos);
void ConvertCoordinatesIfRequired(float &xPos, float &yPos);
bool NeedsOrthoSet();
void ResetOrthoFlag();
void SetOrthoModeFlag();

bool InLandscapeGUIMode(); //use this for things solely about screen size, so windows fake landscape modes can test GUI things
int GetOrientation(); //if three, iPhone is in landscape mode and things will be rotated
float SinPulseByMS(int ms);
float SinGamePulseByMS(int ms); //will pause when the game is paused
float SinToZeroToOneRange(float sinIn); //a helper to use with the above functions when you just want 0-1 range

uint32 ColorCombineMix(uint32 c1, uint32 c2, float progress);
uint32 ColorCombine(uint32 c1, uint32 c2, float alphaMod);
void SetLockedLandscape(bool bNew);
bool GetLockedLandscape();
void  ScreenToWorld(CL_Vec2f pt, CL_Vec3f *pReturnA, float dist);
CL_Vec3f GetOGLPos(int x, int y, float z, CL_Vec3f *pNormalOut, CL_Mat4f *pModelMatrix);
bool CanRotateTo(int orientation);
bool GetIsRotating();
void SetIsRotating(bool bNew) ;
bool GetIsUsingNativeUI();
void SetIsUsingNativeUI(bool bNew);
Entity * GetEntityWithNativeUIFocus();
void SetEntityWithNativeUIFocus(Entity *pEnt);
uint32 GetBrightColor();
bool IsLargeScreen();
float iPhoneMapX(float x);//enter coords for iPhone, if iPad, will convert to relative position (makes porting easier)
float iPhoneMapY(float y); //enter coords for iPhone, if iPad, will convert to relative position (makes porting easier)
CL_Vec2f iPhoneMap( CL_Vec2f vPos ); //enter coords for iPhone, if iPad, will convert to relative position (makes porting easier)
CL_Vec2f iPhoneMap( float x, float y ); //enter coords for iPhone, if iPad, will convert to relative position (makes porting easier)

float iPadMapY( float y );
float iPadMapX( float x );
CL_Vec2f iPadMap( CL_Vec2f vPos );
CL_Vec2f iPadMap( float x, float y );

//like above, but force iPad's/large screens to return numbers 2x of iPhone, useful for sharing the 2x media between iPhone4 and ipad
float iPhoneMapX2X( float x );
float iPhoneMapY2X( float y );
CL_Vec2f iPhoneMap2X( CL_Vec2f vPos );
CL_Vec2f iPhoneMap2X( float x, float y );

void SetupFakePrimaryScreenSize(int x, int y);
int GetFakePrimaryScreenSizeX();
int GetFakePrimaryScreenSizeY();
int GetOriginalScreenSizeY();
int GetOriginalScreenSizeX();
void UndoFakeScreenSize();
void RedoFakeScreenSize();
bool IsTabletSize();
rtRectf ConvertFakeScreenRectToReal(rtRectf r);
eOrientationMode GetForcedOrientation();
void SetForcedOrientation(eOrientationMode orientation);
bool NeedToUseFakeScreenSize(); //true if using a fake (stretched to device) screen and it's a different size than our primary screen
CL_Rectf GetScreenRect();
#endif // RenderUtils_h__
