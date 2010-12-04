/*
 *  RenderUtils.cpp
 *  
 *
 *  Created by Seth Robinson on 3/18/09.
 *  Copyright 2009 Robinson Technologies. All rights reserved.
 *
 */

#include "PlatformPrecomp.h"
#include "RenderUtils.h"
#include "BaseApp.h"

//defaults
int g_screenSizeY = GetPrimaryGLY();
int g_screenSizeX = GetPrimaryGLX();
int g_fakePrimaryScreenSizeX = 0;
int g_fakePrimaryScreenSizeY = 0;
int g_originalScreenSizeX = 0;
int g_originalScreenSizeY = 0;


int g_orientation = ORIENTATION_PORTRAIT;
bool g_lockedLandscape = false;
extern bool g_landScapeNoNeckHurtMode;

void RenderGLTriangle()
{
	//let's draw a simple triangle 
	/*
	 2
	0 1
	*/

	const float triSize = 0.4f;

	GLfloat vVertices[] = 
	{
		-triSize, -triSize, 0.0f,
		triSize, -triSize, 0.0f,
		0.0f, triSize, 0.0f
	};

	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT,  0, vVertices);

	unsigned int rgba = MAKE_RGBA(255,0,0,255);
	glColor4x( (rgba >>8 & 0xFF)*256,  (rgba>>16& 0xFF)*256, (rgba>>24& 0xFF)*256, (rgba&0xFF)*256);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//fix the color back to white
	glColor4x(1 << 16, 1 << 16, 1 << 16, 1 << 16);

	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnable(GL_TEXTURE_2D);
}


void RenderSpinningTriangle()
{
	
	glPushMatrix();
	glTranslatef(0,0,-2);
	glRotatef(float( (GetBaseApp()->GetGameTick()/10) %360) , 0, 1, 0); //rotate it
	//RenderGLTriangle();
	glDisable(GL_CULL_FACE); //so we can see the back of the triangle too
	RenderGLTriangle();
	glEnable(GL_CULL_FACE); //so we can see the back of the triangle too
	glPopMatrix();
	CHECK_GL_ERROR();
}

void RenderTexturedGLTriangle()
{
	//let's draw a simple triangle 
	/*
	 2
	0 1
	*/

	const float triSize = 0.4f;

	GLfloat vVertices[] = 
	{
		-triSize, -triSize, 0.0f,
		triSize, -triSize, 0.0f,
		0.0f, triSize, 0.0f
	};

	GLfloat vTexCoords[] = 
	{
		0,0,
		1, 0,
		0.5,1
	};
	//unsigned int rgba = MAKE_RGBA(255,255,255,255);
	//glColor4x( (rgba >>8 & 0xFF)*256,  (rgba>>16& 0xFF)*256, (rgba>>24& 0xFF)*256, (rgba&0xFF)*256);
	glVertexPointer(3, GL_FLOAT,  0, vVertices);
	glTexCoordPointer(2, GL_FLOAT,  0, vTexCoords);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

}


void RenderTexturedRectangle(float RectSize)
{
	//let's draw a simple rectangle
	/*
	3 2
	0 1
	*/

	GLfloat vVertices[] = 
	{
		-RectSize, -RectSize, 0.0f,
		RectSize, -RectSize, 0.0f,
		RectSize, RectSize, 0.0f,
		-RectSize, RectSize, 0.0f
	};

	GLfloat vTexCoords[] = 
	{
		0,0,
		1,0,
		1, 1,
		0, 1
	};
	//unsigned int rgba = MAKE_RGBA(255,255,255,255);
	//glColor4x( (rgba >>8 & 0xFF)*256,  (rgba>>16& 0xFF)*256, (rgba>>24& 0xFF)*256, (rgba&0xFF)*256);
	glVertexPointer(3, GL_FLOAT,  0, vVertices);
	glTexCoordPointer(2, GL_FLOAT,  0, vTexCoords);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
}

void SetupFakePrimaryScreenSize(int x, int y)
{
	g_fakePrimaryScreenSizeX = x;
	g_fakePrimaryScreenSizeY = y;
	
	//recompute it
	SetupScreenInfo(GetScreenSizeX(), GetScreenSizeY(), GetOrientation());
};


void SetupOriginalScreenSize(int x, int y)
{
	g_originalScreenSizeX = x;
	g_originalScreenSizeY = y;

};

int GetOriginalScreenSizeX()
{
	return g_originalScreenSizeX;
}

int GetOriginalScreenSizeY()
{
	return g_originalScreenSizeY;
}

int GetFakePrimaryScreenSizeX()
{
	
	if (!g_landScapeNoNeckHurtMode)
	{
		return g_fakePrimaryScreenSizeY;
	}
	
	return g_fakePrimaryScreenSizeX;
}

int GetFakePrimaryScreenSizeY()
{

	if (!g_landScapeNoNeckHurtMode)
	{
		return g_fakePrimaryScreenSizeX;
	}

	return g_fakePrimaryScreenSizeY;
}

int g_undoFakeScreenSizeX = 0;
int g_undoFakeScreenSizeY = 0;

void UndoFakeScreenSize()
{
	if (GetFakePrimaryScreenSizeX() == 0) return; //not used

	g_undoFakeScreenSizeX = g_fakePrimaryScreenSizeX;
	g_undoFakeScreenSizeY = g_fakePrimaryScreenSizeY;

	g_fakePrimaryScreenSizeX = 0;
	g_fakePrimaryScreenSizeY = 0;

	g_screenSizeX = GetOriginalScreenSizeX();
	g_screenSizeY = GetOriginalScreenSizeY();
	
	SetupOriginalScreenSize(0,0);

	SetupScreenInfo(g_screenSizeX,g_screenSizeY, GetOrientation());
	PrepareForGL(); //make sure any ortho settings are also updated
}


void RedoFakeScreenSize()
{

	if (g_undoFakeScreenSizeX)
	{
		SetupFakePrimaryScreenSize(g_undoFakeScreenSizeX, g_undoFakeScreenSizeY);
		SetupScreenInfo(GetOriginalScreenSizeX(), GetOriginalScreenSizeY(), GetOrientation());

		g_undoFakeScreenSizeX = 0;
		g_undoFakeScreenSizeY = 0;
	
		PrepareForGL(); //make sure any ortho settings are also updated
	}

}


int GetScreenSizeY()
{
	return g_screenSizeY;
}

int GetScreenSizeX()
{
	return g_screenSizeX;
}

float GetScreenSizeYf()
{
	return (float)g_screenSizeY;
}

float GetScreenSizeXf()
{
	return (float)g_screenSizeX;
}

CL_Vec2f GetScreenSize()
{
	return CL_Vec2f((float)g_screenSizeX, (float)g_screenSizeY);
}

//true for anything larger than an iPhone
bool IsLargeScreen()
{
	return g_screenSizeX>480 || g_screenSizeY > 480;
}

int GetOrientation() {return g_orientation;}

bool GetLockedLandscape() {return g_lockedLandscape;}

void SetLockedLandscape(bool bNew)
{
	if (g_lockedLandscape != bNew)
	{
		g_lockedLandscape = bNew;
		if (GetOrientation() == ORIENTATION_PORTRAIT || GetOrientation() == ORIENTATION_PORTRAIT_UPSIDE_DOWN)
		{
			LogMsg("Forcing landscape mode");
		#if defined(WIN32)
			if (!g_landScapeNoNeckHurtMode)
			{
				SetupScreenInfo(GetScreenSizeY(), GetScreenSizeX(), ORIENTATION_LANDSCAPE_LEFT);

			}
		#else
			SetupScreenInfoIPhone(ORIENTATION_LANDSCAPE_LEFT);
		#endif
			}
		}
	


}


bool g_usingNativeUI = false;
Entity *g_pGUIWithGUIFocus = NULL;

Entity * GetEntityWithNativeUIFocus()
{
	return g_pGUIWithGUIFocus;
}

void SetEntityWithNativeUIFocus(Entity *pEnt)
{
	
	if (pEnt == g_pGUIWithGUIFocus) return;

	if (pEnt)
	{
		if (g_pGUIWithGUIFocus)
		{
			g_pGUIWithGUIFocus->GetShared()->CallFunctionIfExists("OnLosingNativeGUIFocus", NULL);
		}
	}
	
	g_pGUIWithGUIFocus = pEnt;
}

bool GetIsUsingNativeUI() {return g_usingNativeUI;}

void SetIsUsingNativeUI(bool bNew) 
{
	if (bNew == g_usingNativeUI) return;

	if (bNew)
	{
	
	} else
	{

	}
	g_usingNativeUI = bNew;
}


bool CanRotateTo(int orientation)
{
	if (orientation ==  4 || orientation == 3)
	{
		return true;
	}
	if (GetLockedLandscape()) return false;

	return true;
}

bool SetupScreenInfoIPhone(int interfaceOrientation)
{
	
	//note, this is slightly different from our usual orientation (UIDeviceOrientation) so let's convert it first
	int orientation = interfaceOrientation;
	
	if (!CanRotateTo(orientation)) return false;

	if (orientation ==  4 || orientation == 3)
	{
		SetupScreenInfo(GetPrimaryGLY(), GetPrimaryGLX(), orientation);
		return false;
	}
	
	SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(),orientation); 	
	return false;
}

void SetupScreenInfo(int x, int y, int orientation)
{
		SetupOriginalScreenSize(x, y);

/*	
		if (g_screenSizeX == x && g_screenSizeY == y && g_orientation == orientation)
		{
			return;
		}
		*/

#ifdef _DEBUG
		LogMsg("Setting screen info to %d, %d, mode %d.  Original is %d, %d", x, y, orientation, GetOriginalScreenSizeX(), GetOriginalScreenSizeY());
#endif

		g_screenSizeX = x;
		g_screenSizeY = y;
		g_orientation = orientation;
		
		if (GetFakePrimaryScreenSizeX())
		{
			//recompute it using our fake information
			if (x == GetPrimaryGLX())
			{
				y = GetFakePrimaryScreenSizeX();
				x = GetFakePrimaryScreenSizeY();

			} else
			{
				//reversed
				x = GetFakePrimaryScreenSizeX();
				y = GetFakePrimaryScreenSizeY();

			}
			g_screenSizeX = x;
			g_screenSizeY = y;

		}

		NotifyOSOfOrientationPreference(eOrientationMode(orientation));
		if (IsBaseAppInitted())
		GetBaseApp()->OnScreenSizeChange();
}

void ConvertCoordinatesIfRequired(int &xPos, int &yPos)
{
	float x = (float)xPos;
	float y = (float)yPos;
	ConvertCoordinatesIfRequired(x, y);
	xPos = (int)x;
	yPos = (int)y;
}

void ConvertCoordinatesIfRequired(float &xPos, float &yPos)
{
//	LogMsg("Before converting, coords are %d, %d", int(xPos), int(yPos));	

#ifdef __APPLE__

	if (IsIphone4())
	{
		//TODO: Change to use actual scaling value returned by device, who knows when Apple will introduce new scaling values
		xPos *= 2;
		yPos *= 2;
	}
#endif	

	if (GetBaseApp()->GetManualRotationMode())
	{
		switch (GetOrientation())
		{

		case ORIENTATION_PORTRAIT:
			break;

		case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
			yPos = float(GetOriginalScreenSizeY())-yPos;
			xPos = float(GetOriginalScreenSizeX())-xPos;
			break;

		case ORIENTATION_LANDSCAPE_LEFT:
			swap(xPos, yPos);
			yPos = float(GetOriginalScreenSizeY())-yPos;
			break;

		case ORIENTATION_LANDSCAPE_RIGHT:
			yPos = float(GetOriginalScreenSizeX())-yPos;
			swap(xPos, yPos);
			break;

		}
	}
	if (GetFakePrimaryScreenSizeX() != 0)
	{
		//remap to correct values
		//LogMsg("CurY: %.2f - old y: %.2f",  GetScreenSizeYf(), float(GetOriginalScreenSizeY()));
		//LogMsg("CurX: %.2f - old x: %.2f",  GetScreenSizeXf(), float(GetOriginalScreenSizeX()) );
		xPos = (float(xPos) * (GetScreenSizeXf()/float(GetOriginalScreenSizeX())));
		yPos = (float(yPos) * (GetScreenSizeYf()/float(GetOriginalScreenSizeY())));
	}
	
//	LogMsg("Converted coords to %d, %d", int(xPos), int(yPos));
}

bool g_needsOrthoModeSet = true;

bool NeedsOrthoSet() 
{
	return g_needsOrthoModeSet;
}

void ResetOrthoFlag()
{
	
	g_needsOrthoModeSet = true;
}

void SetOrthoModeFlag()
{
	g_needsOrthoModeSet = false;
}

bool InLandscapeGUIMode()
{
	return GetOrientation() == 3 || GetOrientation() == 4 || GetScreenSizeX() >= GetScreenSizeY();
}

//returns a range of -1 to 1 with the cycle matching the MS sent in, based on a sin wave
float SinPulseByMS(int ms)
{
	int tick = GetBaseApp()->GetTick()%ms;
	return (float)(sin (   (float(tick)/float(ms))  *M_PI*2   ));
}

float SinGamePulseByMS(int ms)
{
	int tick = GetBaseApp()->GetGameTick()%ms;
	return (float)(sin (   (float(tick)/float(ms))  *M_PI*2   ));
}

//a helper to use with the above functions when you just want 0-1 range
float SinToZeroToOneRange(float sinIn)
{
	return (sinIn+1.0f)/2;
}

//just makes a random brightish color
uint32 GetBrightColor()
{
	float power = 255;
	int color[3];

	int num = Random(2);

	if (Random(3))
	{
		color[num%3] = Random(80)+ (255-80); 

	} else
	{
		color[num%3] = Random(150)+ (255-150); 

	}
	color[(num+1)%3] = rt_min(255, Random(355 - (color[num%3])));
	color[(num+2)%3] =  rt_min(255,Random(455 - (color[num%3] +color[(num+1)%3] )));
	return MAKE_RGBA(color[0], color[1], color[2], 255);
}

//mix between c1 and c2 based on progress, which should be 0 to 1
uint32 ColorCombineMix(uint32 c1, uint32 c2, float progress)
{
	int r,g,b,a;

	//OPTIMIZE oh come on, optimize this - Seth

	r = int (float(GET_RED(c1)) + ((  float(GET_RED(c2)) - float(GET_RED(c1))  ) * progress));
	g = int (float(GET_GREEN(c1)) + ((  float(GET_GREEN(c2)) - float(GET_GREEN(c1))  ) * progress));
	b = int (float(GET_BLUE(c1)) + ((  float(GET_BLUE(c2)) - float(GET_BLUE(c1))  ) * progress));
	a = int (float(GET_ALPHA(c1)) + ((  float(GET_ALPHA(c2)) - float(GET_ALPHA(c1))  ) * progress));


	//LogMsg(PrintColor(MAKE_RGBA(r,g,b,a)).c_str());
	return MAKE_RGBA(r,g,b,a);
}

//modifies c1 with c2, also applies alphaMod at the end
uint32 ColorCombine(uint32 c1, uint32 c2, float alphaMod)
{
	int r,g,b,a;

	if (c2 != MAKE_RGBA(255,255,255,255))
	{
		r = GET_RED(c1) - (255- GET_RED(c2));
		g = GET_GREEN(c1) - (255-  GET_GREEN(c2));
		b = GET_BLUE(c1) - (255-  GET_BLUE(c2));
		a = GET_ALPHA(c1) - (255-  GET_ALPHA(c2));

		r = max(0, r); r = min(255,r);
		g = max(0, g); g = min(255,g);
		b = max(0, b); b = min(255,b);
		a = max(0, a); a = min(255,a);
	} else
	{
		if (alphaMod == 1) return c1;
		r = GET_RED(c1);
		g = GET_GREEN(c1);
		b = GET_BLUE(c1); 
		a = GET_ALPHA(c1);
	}

	//LogMsg("%s and %s make %s", PrintColor(c1).c_str(), PrintColor(c2).c_str(), PrintColor(MAKE_RGBA(r,g,b, byte(float(a)*alphaMod))).c_str());
	return MAKE_RGBA(r,g,b, byte(float(a)*alphaMod)) ;
}

void  ScreenToWorld(CL_Vec2f pt, CL_Vec3f *pReturnA, float dist)
{
	float aspect = GetScreenSizeYf()/GetScreenSizeXf();

	const float tanffov =  tanf((float(C_APP_FOV) * M_PI) / 360.0f);

	float x =  pt.x / (GetScreenSizeXf()/2)-1.0f;
	float y = 1.0f- (pt.y / (GetScreenSizeYf()/2));

	x = tanffov * (x) / aspect;
	y =  tanffov * y;

	CL_Mat4f mat16, projMat16;
	glGetFloatv(GL_MODELVIEW_MATRIX, mat16);
	glGetFloatv(GL_PROJECTION_MATRIX, projMat16);

	mat16.inverse();
	
	projMat16.inverse();

	*pReturnA = CL_Vec3f(x,y,dist);
	
	CL_Vec4f vTemp4 = CL_Vec4f(    pReturnA->x,    pReturnA->y,  pReturnA->z, 1)*mat16;
	vTemp4 = vTemp4*projMat16;

	*pReturnA = CL_Vec3f(  vTemp4.x,  vTemp4.y, vTemp4.z);
}

CL_Vec3f GetOGLPos(int x, int y, float z, CL_Vec3f *pNormalOut, CL_Mat4f *pModelMatrix)
{
		
	//this xmod/ymod handles some things we do with auto screen stretching if "fake screensize" is used

	float xmod = (float(GetOriginalScreenSizeX())/GetScreenSizeXf());
	float ymod = (float(GetOriginalScreenSizeY())/GetScreenSizeYf());

	if (GetBaseApp()->GetManualRotationMode())
	{
		
		switch (GetOrientation())
		{
		case ORIENTATION_LANDSCAPE_LEFT:
			y = GetScreenSizeY()-y;
			swap(x,y);
			swap(xmod,ymod);
			break;

		case ORIENTATION_LANDSCAPE_RIGHT:
			x = GetScreenSizeX()-x;
			swap(x,y);
			swap(xmod,ymod);
			break;

		case ORIENTATION_PORTRAIT:
			break;

		case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
			y = GetScreenSizeY()-y;
			x = GetScreenSizeX()-x;
			break;
		}
	}
	
	//convert back into real screen coordinates if needed

	if (GetFakePrimaryScreenSizeX() != 0)
	{
		//remap to correct values
		x = (int) (float(x) * xmod);
		y = (int) (float(y) * ymod);
	}

	GLint viewport[4];
	GLfloat modelview[16];
	GLfloat projection[16];
	GLfloat winX, winY;
	GLfloat posX, posY, posZ;

	*((CL_Mat4f*)modelview) = *pModelMatrix;
	*((CL_Mat4f*)projection) = *GetBaseApp()->GetProjectionMatrix();
	//glGetFloatv( GL_PROJECTION_MATRIX, projection );
	
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = GetPrimaryGLX();
	viewport[3] = GetPrimaryGLY();
	//glGetIntegerv( GL_VIEWPORT, viewport ); //doesn't work on android

	//Read the window z co-ordinate 
	//(the z value on that point in unit cube)		
	
	CL_Vec3f a,b;
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	gluUnProject( winX, winY, 0, modelview, projection, viewport, &posX, &posY, &posZ);
	a = CL_Vec3f(posX, posY, posZ);

	gluUnProject( winX, winY, 1, modelview, projection, viewport, &posX, &posY, &posZ);
	b = CL_Vec3f(posX, posY, posZ);

	CL_Vec3f n = b-a;
	n.normalize();
	if (pNormalOut)
	{
		*pNormalOut = n;
	}
	return a + (n* z);
}


float iPhoneMapY( float y )
{
	return (int)(y*GetScreenSizeYf())/320.0f; //I cast to int because it helps with "middle of the pixel" jaggies
}

float iPhoneMapX( float x )
{
	return (int)(x*GetScreenSizeXf())/480.0f;  //I cast to int because it helps with "middle of the pixel" jaggies
}

CL_Vec2f iPhoneMap( CL_Vec2f vPos )
{
	return CL_Vec2f((int) (vPos.x*GetScreenSizeXf()/480.0f), (int) (vPos.y*GetScreenSizeYf()/320.0f));
}

CL_Vec2f iPhoneMap( float x, float y )
{
	return CL_Vec2f((int) (x*GetScreenSizeXf()/480.0f), (int) (y*GetScreenSizeYf()/320.0f));
}

float iPhoneMapX2X( float x )
{
	if (!IsLargeScreen()) return x;
	return (int)(x*960)/480;
}


float iPhoneMapY2X( float y )
{
	if (!IsLargeScreen()) return y;
	return (int)(y*649)/320;
}

CL_Vec2f iPhoneMap2X( CL_Vec2f vPos )
{
	if (!IsLargeScreen()) return vPos;
	
	return CL_Vec2f((int) (vPos.x*960/480), (int) (vPos.y*640/320));
}

CL_Vec2f iPhoneMap2X( float x, float y )
{
	if (!IsLargeScreen()) return CL_Vec2f(x, y);
	return CL_Vec2f((int) (x*960/480), (int) (y*640/320));
}




