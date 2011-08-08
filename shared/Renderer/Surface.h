//  ***************************************************************
//  Surface - Creation date: 03/24/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Surface_h__
#define Surface_h__

#include "util/RTFileFormat.h"
#include "util/RenderUtils.h"
#include "util/MiscUtils.h"

#include "util/boost/boost/signal.hpp"

struct glColorBytes
{
	glColorBytes(){};
	glColorBytes(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a) : r(_r), g(_g), b(_b), a(_a){};

	bool Compare(const glColorBytes &c) {return *(int*)&r == *(int*)&c.r;} //I couldn't get == working on this struct, a weird GUID error under windows?!, so I did this..
	GLubyte r,g,b,a;
};



#define NO_TEXTURE_LOADED (2000000000) //I had this as 4294967294u but saw some weirdness with some compiles

class Surface: public boost::signals::trackable
{
public:

	enum eTextureType
	{
		TYPE_DEFAULT, //normal, enables wrapping, mips
		TYPE_GUI, //no texture wrapping (uses clamp), disables mip creation/loading
		TYPE_NOT_OWNER, //do some work for irrlicht, don't actually bind/unbind things
		TYPE_NO_SMOOTHING //no linear smoothing filters are applied
	};

	enum eBlendingMode
	{
		BLENDING_NORMAL,
		BLENDING_ADDITIVE,
		BLENDING_PREMULTIPLIED_ALPHA
	};

	
	Surface();
	Surface (string fName); //load in an image right away
	Surface (string fName, eTextureType type); //load in an image right away
	virtual ~Surface();

	
	bool LoadFile(string fName); //will autodetect what kind of file it is, rttext or .rttex supported
	virtual bool LoadFileFromMemory(byte *pMem); //will autodetect what kind of file it is, rttext or .rttex supported
	void Bind();
	bool IsLoaded() {return m_glTextureID != NO_TEXTURE_LOADED || !m_textureLoaded.empty();}
	bool UsesAlpha() {return m_bUsesAlpha;}
	void SetUsesAlpha(bool bNew){m_bUsesAlpha = bNew;}

	int GetWidth() {return m_originalWidth;}
	int GetHeight() {return m_originalHeight;}

	int GetRawTextureWidth() {return m_texWidth;}
	int GetRawTextureHeight() {return m_texHeight;}

	void SetTextureType(eTextureType type);

	void Blit(  float x, float y, unsigned int rgba = MAKE_RGBA(255,255,255,255), float rotation = 0, CL_Vec2f vRotatePt = CL_Vec2f(0,0));
	void BlitScaled( float x, float y, CL_Vec2f vScale, eAlignment alignment = ALIGNMENT_CENTER, unsigned int rgba  = MAKE_RGBA(255,255,255,255), float rotation=0);
	void BlitScaledWithRotatePoint( float x, float y, CL_Vec2f vScale, eAlignment alignment, unsigned int rgba, float rotation, CL_Vec2f vRotationPt);
	void BlitEx(rtRectf dst, rtRectf src, unsigned int rgba = MAKE_RGBA(255,255,255,255), float rotation = 0, CL_Vec2f vRotatePt = CL_Vec2f(0,0)); //more advanced version, can do scaling and sub-texture blits
	rtRectf GetRectf() {return rtRectf(0,0, float(m_originalWidth), float(m_originalWidth));}
	void SetSmoothing( bool bSmoothing);
	void SetBlendingMode(eBlendingMode mode) {m_blendingMode = mode;}
	eBlendingMode GetBlendingMode() {return m_blendingMode;}
	int GetMIPMapCount() {return m_mipMapCount;}

	virtual bool InitBlankSurface(int x, int y); //initialize a blank surface to do whatever to
	bool IsRenderTarget() {return m_frameBuffer != 0;}
	void CopyFromScreen();
	void UpdateSurfaceRect(rtRect dstRect, byte *pPixelData, bool bUpsideDownMode = true);
	void FillColor(glColorBytes color);
	virtual void Kill();
	void OnLoadSurfaces();
	void OnUnloadSurfaces();

protected:
	virtual void ReloadImage();

	enum eTextureCreationMethod //helps me to know how to restore it when losing surfaces
	{
		TEXTURE_CREATION_NONE, //we haven't made a texture yet
		TEXTURE_CREATION_FILE, //we'll reload automatically
		TEXTURE_CREATION_MEMORY, //we'll lose it, but not restore it
		TEXTURE_CREATION_BLANK //we'll reinitialize the texture as blank, up to you to redraw it
	};

private:

	bool LoadBMPTexture(byte *pMem); //handles 32 bit (alpha ok) and 24 bit, no alpha
	bool LoadRTTexture(byte *pMem); //must be created with the RTPack.exe utility
	void IncreaseMemCounter(int mem);
	void SetTextureStates();
	void PrepareGLForNewTexture();
	void SetDefaults();
	void PreMultiplyAlpha(byte *pBytes, int width, int height, int format);
	GLuint m_glTextureID;
	int m_texWidth, m_texHeight;
	int m_originalWidth, m_originalHeight; //if the texture is padded, this info helps us to still draw it as if it wasn't
	bool m_bUsesAlpha;
	eTextureType m_texType;
	eBlendingMode m_blendingMode;
	int m_mipMapCount;
	GLuint m_frameBuffer;
	int m_memUsed; //for internal memory monitering
	string m_textureLoaded;
	eTextureCreationMethod m_textureCreationMethod;
	
};

extern GLuint g_lastBound;
#endif // Surface_h__