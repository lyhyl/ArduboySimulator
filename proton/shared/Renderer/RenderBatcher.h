//  ***************************************************************
//  RenderBatcher - Creation date: 08/31/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef RenderBatcher_h__
#define RenderBatcher_h__

#include "SurfaceAnim.h"


class SoftSurface;

struct BatchVert
{
	CL_Vec3f vPos;
	CL_Vec2f vUv;
	CL_Vec3f vNormal;
	glColorBytes color;
};

class RenderBatcher
{
public:
	RenderBatcher();
	virtual ~RenderBatcher();
	void BlitEx(Surface *pSurf, rtRectf dst, rtRectf src, unsigned int rgba = MAKE_RGBA(255,255,255,255));
	void Flush();

	void BlitRawImage(int dstX, int dstY, byte *pRaw, int width, int height, bool bNeedsAlpha);
	void BlitRawImage(int x, int y, SoftSurface &soft);

	void glDrawArrays(const CL_Vec3f *pVerts, const CL_Vec3f *vNormals, const CL_Vec2f *TexCords, glColorBytes glColorByte,
		CL_Mat4f *pMatrix, int glDrawMode, int primCount);

	void Flush3D(bool bUseNormals, bool bUseTextures, bool bUseColorByte);
	void BuildVert(BatchVert *pVert, const CL_Vec3f *pVerts, const CL_Vec3f *vNormals, const CL_Vec2f *TexCords, glColorBytes glColorByte, CL_Mat4f *pMatrix, int srcVertIndex);
protected:
	
	Surface *m_pSurf;
	vector<BatchVert> m_verts;

private:
};


#endif // RenderBatcher_h__