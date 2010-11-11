#include "PlatformPrecomp.h"
#include "Surface.h"
#include "bitmap.h"
#include "BaseApp.h"
#include "RTGLESExt.h"
 
#include <cmath>

#define GL_RGBA8 0x8058

GLuint g_lastBound = 0;

Surface::Surface()
{
	SetDefaults();

}

Surface::Surface( string fName )
{
	SetDefaults();
	LoadFile(fName);
}

Surface::Surface( string fName, eTextureType type )
{
	SetDefaults();
	SetTextureType(type);
	LoadFile(fName);
}

void Surface::SetDefaults()
{
	m_texWidth = 0;
	m_texHeight = 0;
	m_memUsed = 0;
	m_glTextureID = NO_TEXTURE_LOADED;
	m_texType = TYPE_DEFAULT;
	m_blendingMode = BLENDING_NORMAL;
	m_mipMapCount = 0;
	m_frameBuffer = 0;
	m_textureCreationMethod = TEXTURE_CREATION_NONE;
}

Surface::~Surface()
{
	Kill();
}

void Surface::Kill()
{
	if (m_glTextureID != NO_TEXTURE_LOADED)
	{
		glDeleteTextures( 1, &m_glTextureID );
#ifdef _DEBUG
		//LogMsg("Killing texture %s", m_textureLoaded.c_str());
#endif
		m_glTextureID = NO_TEXTURE_LOADED;
		GetBaseApp()->ModTexUsed(-m_memUsed);
		m_memUsed = 0;
	}
}

bool Surface::LoadBMPTexture(byte *pMem)
{
	BMPFileHeader *pBmpHeader = (BMPFileHeader*)&pMem[0];
	BMPImageHeader *pBmpImageInfo = (BMPImageHeader*)&pMem[14];

	unsigned short offsetToImageData;
	
	memcpy(&offsetToImageData, &pMem[10], 2);
	byte *pPixelData = &pMem[offsetToImageData];
	
	if (!IsPowerOf2(pBmpImageInfo->Width) || !IsPowerOf2(pBmpImageInfo->Height))
	{
		LogError("Bitmap dimensions needs to be of a power of 2, use RTPack on it first, this way it can still be used as if it was its original size");
		LogError("Or, use SoftSurface which can do this and has a better bmp loader in it.");
		return false;
	}
	m_texWidth = pBmpImageInfo->Width;
	m_texHeight = pBmpImageInfo->Height;

	m_originalWidth = m_texWidth;
	m_originalHeight = m_texHeight;

	GLenum colorFormat = GL_RGBA;
	GLenum pixelFormat = GL_UNSIGNED_BYTE;
	m_mipMapCount = 0;
	//we're going to have to convert it to 5551 or 4444 on our own

	uint16 *pFinal = 0;
	uint8 *pPixelDataToUse = pPixelData;
	
	//note, yes, bmps are stored upside down, but so do our gl textures so we don't flip the Y

	if (pBmpImageInfo->BitCount == 32)
	{
		//convert BGRA to RGBA.  I think there is actually an undocumented parm to make the iphone except RGBA but whatever
		int pixelCount = m_texWidth*m_texHeight;
		byte temp;
		for (int i=0; i < pixelCount; i++)
		{
		  temp = *pPixelData;
		  *pPixelData = pPixelData[2];
		  pPixelData[2] = temp;
		  pPixelData += 4;
		}
	} else
		if (pBmpImageInfo->BitCount == 24)
		{
			colorFormat = GL_RGB;		
			//convert BGR to RGB. 
			int pixelCount = m_texWidth*m_texHeight;
			byte temp;
			for (int i=0; i < pixelCount; i++)
			{
				temp = *pPixelData;
				*pPixelData = pPixelData[2];
				pPixelData[2] = temp;
				pPixelData += 3;
			}
		} 

	 else
	{
		LogError("Don't handle %d bit bmps yet", pBmpImageInfo->BitCount);
		return false;
	}
	
	PrepareGLForNewTexture();	
	m_bUsesAlpha = (colorFormat == GL_RGBA);
	glTexImage2D( GL_TEXTURE_2D, 0, colorFormat, m_texWidth, m_texHeight, 0, colorFormat, pixelFormat, pPixelDataToUse );
	IncreaseMemCounter(m_texWidth*m_texHeight* (pBmpImageInfo->BitCount/8));
	SetTextureStates();
	CHECK_GL_ERROR();
	SAFE_DELETE_ARRAY(pFinal);
	return true;
}

void Surface::IncreaseMemCounter(int mem)
{
	if (m_texType == TYPE_NOT_OWNER) return;

	assert(!m_memUsed && "Forgot to clear this somewhere?");
	m_memUsed = mem;
	GetBaseApp()->ModTexUsed(m_memUsed);
}
void Surface::PrepareGLForNewTexture()
{
	
	if (m_texType == TYPE_NOT_OWNER) return;

	CHECK_GL_ERROR();

	glGenTextures( 1, &m_glTextureID ); 
	CHECK_GL_ERROR();
	glBindTexture( GL_TEXTURE_2D, m_glTextureID);
	g_lastBound = m_glTextureID;
	CHECK_GL_ERROR();

	
	//glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
}

#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG			0x8C00 //35840
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG			0x8C01 //35841
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02 //35842
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03 //35843

#ifndef GL_UNSIGNED_SHORT_4_4_4_4
	#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#endif


void Surface::PreMultiplyAlpha(byte *pBytes, int width, int height, int format)
{
	assert(format == GL_UNSIGNED_SHORT_4_4_4_4  || format == GL_UNSIGNED_BYTE && "This doesn't make sense premuliplying something that has no alpha!");

	if (format == GL_UNSIGNED_SHORT_4_4_4_4)
	{
		uint16 *pDestImage = (uint16 *)pBytes;

		int r,g,b,a;

		for (int y=0; y < width; y++)
		{
			for (int x=0; x < height; x++)
			{
				
				//get the alpha
				a = (*pDestImage >> 0) & 15;
				
				/*
				//without modifying it, we'd do this:
				r = (((*pDestImage >> 0) & 15);
				g = ((*pDestImage >> 4) & 15);
				b = ((*pDestImage >> 8) & 15);
				*/
	
				r = (((*pDestImage >> 4) & 15)*a)/16;
				g = (((*pDestImage >> 8) & 15)*a)/16;
				b = (((*pDestImage >> 12) & 15)*a)/16;

				//uint16 orig = *pDestImage;
				//LogMsg("a: %d, r %d g %d b %d", int(a), int(r), int(g), int(b));
				
				//pack it back in
				
				*pDestImage = a << 0 | r << 4 | g << 8 | b << 12;
				pDestImage++;
			}
		}


	} else if (format == GL_UNSIGNED_BYTE)
	{
		glColorBytes *pDestImage = (glColorBytes*)pBytes;

		//slower way that supports transparency
		for (int y=0; y < width; y++)
		{
			for (int x=0; x < height; x++)
			{

				pDestImage->r = (pDestImage->r*pDestImage->a)/255;
				pDestImage->g = (pDestImage->g*pDestImage->a)/255;
				pDestImage->b = (pDestImage->b*pDestImage->a)/255;
				//*pDestImage = glColorBytes(255,255,255,255);
				pDestImage++;
			}
		}
	} else
	{
		LogError("Don't know how to premultiply this alpha");
		assert(0);
	}

	

}

bool Surface::LoadRTTexture(byte *pMem)
{
	rttex_header *pTexHeader = (rttex_header*)pMem;
	rttex_mip_header *pMipSection;
	
	PrepareGLForNewTexture();

	m_texWidth = pTexHeader->width;
	m_texHeight = pTexHeader->height;
	m_bUsesAlpha = pTexHeader->bUsesAlpha != 0;
	int texType = pTexHeader->format;
	m_originalWidth = pTexHeader->originalWidth;
	m_originalHeight = pTexHeader->originalHeight;
	m_mipMapCount = pTexHeader->mipmapCount;
	byte *pCurPos = pMem + sizeof(rttex_header);

	const uint32 PVRTC2_MIN_TEXWIDTH		= 16;
	const uint32 PVRTC2_MIN_TEXHEIGHT		= 8;
	const uint32 PVRTC4_MIN_TEXWIDTH		= 8;
	const uint32 PVRTC4_MIN_TEXHEIGHT		= 8;
	const uint32 ETC_MIN_TEXWIDTH			= 4;
	const uint32 ETC_MIN_TEXHEIGHT		= 4;
	int memUsed = 0;
	
	for (int nMipLevel=0; nMipLevel < m_mipMapCount; nMipLevel++)
	{
	
		pMipSection = (rttex_mip_header*)pCurPos;
		pCurPos += sizeof(rttex_mip_header);
		byte *pTextureData =  (byte*)pCurPos ;
		memUsed += pMipSection->dataSize;
		if (pTexHeader->format < GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG || pTexHeader->format > GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG) //doesn't look like a compressed texture
		{
			int colorType = GL_RGBA;
			if (!m_bUsesAlpha)
			{
				colorType = GL_RGB;
			}
//			LogMsg("Loading surface: miplevel %d, internal color type:0x%02lX  colortype 0x%02lX, x%d y%d, format type: 0x%02lX", nMipLevel, colorTypeSource, colorType, pMipSection->width, pMipSection->height, pTexHeader->format );
			int internalColorFormat = colorType;
#ifdef C_GL_MODE
			if (internalColorFormat == GL_RGBA) internalColorFormat = GL_RGBA8;
			if (internalColorFormat == GL_RGB) internalColorFormat = GL_RGB8;
#endif

			if (m_texType == TYPE_GUI && m_bUsesAlpha)
			{
				//Go ahead and activate premultiplied alpha processing.  This will allow us to zoom in on textures and not have ugly artifacts
				//around the edges.
				m_blendingMode = BLENDING_PREMULTIPLIED_ALPHA;
			}

			if (m_blendingMode == BLENDING_PREMULTIPLIED_ALPHA && m_bUsesAlpha)
			{
				//let's do it manually now
				PreMultiplyAlpha(pTextureData, pMipSection->width, pMipSection->height, pTexHeader->format);

			}

			glTexImage2D( GL_TEXTURE_2D, nMipLevel, internalColorFormat, pMipSection->width, pMipSection->height, 0, colorType, pTexHeader->format, pTextureData );
		} else
		{
			
#ifdef C_GL_MODE

			assert(!"You cannot use PVR compressed textures in GL mode!");
#else
	
			glCompressedTexImage2D(
				GL_TEXTURE_2D, 
				nMipLevel, 
				pTexHeader->format, 
				pMipSection->width, 
				pMipSection->height, 
				0, 
				pMipSection->dataSize, 
				pTextureData);
#endif
		}
	
		pCurPos += pMipSection->dataSize;
		
		CHECK_GL_ERROR();
	}


	if (m_mipMapCount > 1 && m_glTextureID != NO_TEXTURE_LOADED)
	{
		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	}
	
	IncreaseMemCounter(memUsed);
	SetTextureStates();
	return true;
}

bool Surface::LoadFileFromMemory( byte *pMem )
{
	Kill();
	
	if (m_texWidth == 0)
	{
	
		if (m_textureCreationMethod = TEXTURE_CREATION_NONE)
		{
			m_textureCreationMethod = TEXTURE_CREATION_MEMORY;
		} else
		{
			//load on demand, helps with speed
		}
	
		//unload happens right away though
		GetBaseApp()->m_sig_unloadSurfaces.connect(1, boost::bind(&Surface::OnUnloadSurfaces, this));
	
	}

	if (strncmp((char*)pMem, "BM", 2) == 0)
	{
		//we've got a bitmap on our hands it looks like
		return LoadBMPTexture(pMem);
	} else if (strncmp((char*)pMem, C_RTFILE_TEXTURE_HEADER, 6) == 0)
	{
		return LoadRTTexture(pMem);
	} else
	{
		LogError("Surface: Unknown file type");
		return false; 
	}

	return true;
}


void Surface::Bind()
{
	
	if (m_glTextureID == NO_TEXTURE_LOADED && !m_textureLoaded.empty())
	{
		ReloadImage();
	}
	
	if (g_lastBound == m_glTextureID)
	{
		//believe it or not, I saved 2 FPS on the iphone by doing my own checks
		return;
	}
	assert(IsLoaded() && "Shouldn't you load a texture before binding it?");
	glBindTexture( GL_TEXTURE_2D, m_glTextureID);
	g_lastBound = m_glTextureID;
	CHECK_GL_ERROR();

}

bool Surface::LoadFile( string fName )
{
	if (fName.empty())
	{
		Kill();
		return true; //no error, but I guess they just don't want anything loaded right now
	}
	
#ifdef _DEBUG
	LogMsg("Loading texture %s", fName.c_str());
#endif

	FileInstance f(fName);
	if (!f.IsLoaded()) return false;
	
	m_textureLoaded = fName;
	m_textureCreationMethod = TEXTURE_CREATION_FILE;
	return LoadFileFromMemory(f.GetAsBytes());
}

typedef struct
{
	int         u;
	int         v;
	int         w;
	int         h;
} TextureCoords;

struct TexCoords
{
	float x, y;
};

struct Verts
{
	float x,y,z;
};

void Surface::BlitEx(rtRectf dst, rtRectf src, unsigned int rgba, float rotation, CL_Vec2f vRotatePt)
{

	/*
	if (rotation != 0)
	{
		dst.AdjustPosition(-vRotatePt.x, -vRotatePt.y);
		PushRotationMatrix(rotation, vRotatePt);
	}

	g_globalBatcher.BlitEx(this, dst, src, rgba);
	g_globalBatcher.Flush();
	if (rotation != 0)
	{
		PopRotationMatrix();
	}
*/

	if (dst.bottom < 0) return;
	if (dst.top > GetOrthoRenderSizeYf()) return;
	
	if ( GET_ALPHA(rgba) == 0)
	{
		return;	
	}

	if (!IsLoaded()) return;
	
	SetupOrtho(); //upside down, makes this easier to do
	Bind();
	
	//LogMsg("Rendering tex %d at %s at %d", m_glTextureID, PrintRect(dst).c_str(), GetTick(TIMER_GAME));
	
	if (rotation != 0)
	{
		dst.AdjustPosition(-vRotatePt.x, -vRotatePt.y);
		PushRotationMatrix(rotation, vRotatePt);
	}

//	0 1
//	3 2

	static GLfloat	vertices[3*4];

// 	GLfloat	vertices[] = {
// 		dst.left,		dst.top,		0.0,
// 		dst.right,		dst.top,		0.0,
// 		dst.right,		dst.bottom,		0.0,
// 		dst.left,		dst.bottom,		0.0 };
// 		

vertices[0*3+0] = dst.left; vertices[0*3+1] = dst.top;
vertices[1*3+0] = dst.right; vertices[1*3+1] = dst.top;
vertices[2*3+0] = dst.right; vertices[2*3+1] = dst.bottom;
vertices[3*3+0] = dst.left; vertices[3*3+1] = dst.bottom;

//set the Z
vertices[0*3+2] = 0;
vertices[1*3+2] = 0;
vertices[2*3+2] = 0;
vertices[3*3+2] = 0;


		static GLfloat vTexCoords[8];
		//another step to convert the coordinates into ratios
		static float texW;
		texW = float(m_originalWidth)/float(m_texWidth);
		static float texH;
		texH = float(m_originalHeight)/float(m_texHeight);
		static TexCoords *pTex;

		pTex = (TexCoords*)vTexCoords;
		
		//vert 0
		pTex->x = src.left/float(m_originalWidth) * texW;
		pTex->y =  (1-texH) + texH*( (m_originalHeight-src.top) /float(m_originalHeight));
		pTex++;

		//vert 1
		pTex->x = src.right/float(m_originalWidth) * texW;
		pTex->y = (1-texH) + texH*( (m_originalHeight-src.top) /float(m_originalHeight));
		pTex++;
		
		//vert 2
		pTex->x = src.right/float(m_originalWidth) * texW;
		pTex->y = 1-(texH*(src.bottom /(float(m_originalHeight))));
		pTex++;

		//vert 3
		pTex->x = src.left/float(m_originalWidth) * texW;
		pTex->y = 1-(texH*(src.bottom /(float(m_originalHeight))));
		pTex++;

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT,  0, vTexCoords);
		CHECK_GL_ERROR();

		if (UsesAlpha() || rgba != MAKE_RGBA(255,255,255,255) || m_blendingMode == BLENDING_PREMULTIPLIED_ALPHA)
		{
			glEnable( GL_BLEND );
			
			switch (m_blendingMode)
			{
				case BLENDING_NORMAL:
				glColor4x( (rgba >>8 & 0xFF)*256,  (rgba>>16& 0xFF)*256, (rgba>>24& 0xFF)*256, (rgba&0xFF)*256);
				break;


		
			case BLENDING_ADDITIVE:
				glBlendFunc( GL_SRC_ALPHA, GL_ONE);
				glColor4x( (rgba >>8 & 0xFF)*256,  (rgba>>16& 0xFF)*256, (rgba>>24& 0xFF)*256, (rgba&0xFF)*256);

				break;
			
			case BLENDING_PREMULTIPLIED_ALPHA:
				glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
				int alpha = (rgba&0xFF);
				glColor4x( (rgba >>8 & 0xFF)*alpha ,  (rgba>>16& 0xFF)*alpha, (rgba>>24& 0xFF)*alpha, alpha*256);
				break;

			}

		} else
		{
			//glDisable(GL_BLEND);
			//LogMsg("No alpha");
		}

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		CHECK_GL_ERROR();

		if (UsesAlpha() || rgba != MAKE_RGBA(255,255,255,255)|| m_blendingMode == BLENDING_PREMULTIPLIED_ALPHA)
		{
			glColor4x(1 << 16, 1 << 16, 1 << 16, 1 << 16);
			glDisable( GL_BLEND );
	
			if (m_blendingMode != BLENDING_NORMAL)
			{
				//put it back to how it was
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			}
		}

		if (rotation != 0)
		{
			PopRotationMatrix();
		}
}

void Surface::BlitScaled( float x, float y, CL_Vec2f vScale, eAlignment alignment, unsigned int rgba, float rotation)
{
	
	assert(vScale.x != 0 && vScale.y != 0 && "Dahell?");

	CL_Vec2f vStart = CL_Vec2f(x,y);
	rtRectf src(0,0,(float)m_originalWidth,(float)m_originalHeight);
	rtRectf dst = src;
	vStart -= GetAlignmentOffset(CL_Vec2f(float(m_originalWidth), float(m_originalHeight)), eAlignment(alignment));
	dst.AdjustPosition(vStart.x, vStart.y);
	dst.Scale(alignment, vScale);

	BlitEx(dst, src, rgba, rotation, CL_Vec2f(x,y));
}


void Surface::Blit( float x, float y, unsigned int rgba, float rotationDegrees, CL_Vec2f vRotatePt)
{
	SetupOrtho();
	Bind();
	
	if (rotationDegrees != 0)
	{
		x -= vRotatePt.x;
		y -= vRotatePt.y;
		PushRotationMatrix(rotationDegrees, vRotatePt);
	}

	//LogMsg("Rendering tex %d at %.2f ,%.2f at time %d", m_glTextureID, x,y, GetTick(TIMER_GAME));

	GLfloat	vertices[] = {
		x,			y,			0.0,
		x + m_originalWidth,		y,			0.0,
		x + m_originalWidth,		m_originalHeight+y,		0.0,
		x ,			m_originalHeight+y,		0.0 };

		//3 2
		//0 1
		
		float texW = float(m_originalWidth)/float(m_texWidth);
		float texH = float(m_originalHeight)/float(m_texHeight);

		GLfloat vTexCoords[] = 
		{
			0, 1,
			texW, 1,
			texW, 1-texH,
			0, 1-texH
		};
	
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT,  0, vTexCoords);
		CHECK_GL_ERROR();

		assert((rgba&0xFF)*256 != 0 && "Why send something with zero alpha?");
		
		if (UsesAlpha() || rgba != MAKE_RGBA(255,255,255,255)|| m_blendingMode == BLENDING_PREMULTIPLIED_ALPHA)
		{
			glEnable( GL_BLEND );
		
			switch (m_blendingMode)
			{
			
			case BLENDING_NORMAL:
				glColor4x( (rgba >>8 & 0xFF)*256,  (rgba>>16& 0xFF)*256, (rgba>>24& 0xFF)*256, (rgba&0xFF)*256);
				break;

			case BLENDING_ADDITIVE:
				glBlendFunc( GL_SRC_ALPHA, GL_ONE);
				glColor4x( (rgba >>8 & 0xFF)*256,  (rgba>>16& 0xFF)*256, (rgba>>24& 0xFF)*256, (rgba&0xFF)*256);
				break;
			
			case BLENDING_PREMULTIPLIED_ALPHA:
				glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
				int alpha = (rgba&0xFF);
				glColor4x( (rgba >>8 & 0xFF)*alpha ,  (rgba>>16& 0xFF)*alpha, (rgba>>24& 0xFF)*alpha, alpha*256);

				break;

			}
		} else
		{
	//		LogMsg("No blending");
			//glDisable( GL_BLEND );
		}

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		CHECK_GL_ERROR();

	
		if (UsesAlpha() || rgba != MAKE_RGBA(255,255,255,255)|| m_blendingMode == BLENDING_PREMULTIPLIED_ALPHA)
		{
			glColor4x(1 << 16, 1 << 16, 1 << 16, 1 << 16);
			glDisable( GL_BLEND );

			if (m_blendingMode != BLENDING_NORMAL)
			{
				//put it back to how it was
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			}
		}

		if (rotationDegrees != 0)
		{
			PopRotationMatrix();
		}

	

}

void Surface::SetTextureType( eTextureType type )
{
	assert(!IsLoaded() && "You should change this to able to work on an existing texture");
	m_texType = type;
}

void Surface::SetSmoothing( bool bSmoothing )
{
	assert(IsLoaded() && "You should change this to work on things that aren't already loaded as well");
	Bind();
	glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );


}
template <typename T>
T nextPowerOfTwo(T n)
{
	std::size_t k=1;
	--n;
	do {
		n |= n >> k;
		k <<= 1;
	} while (n & (n + 1));
	return n + 1;
}


/*
//! Bind Render Target Texture
void COGLES1Texture::bindRTT()
{
	glViewport(0, 0, getSize().Width, getSize().Height);
}


//! Unbind Render Target Texture
void COGLES1Texture::unbindRTT()
{
	glBindTexture(GL_TEXTURE_2D, getOGLES1TextureName());

	// Copy Our ViewPort To The Texture
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, getSize().Width, getSize().Height);
}
*/


enum eBlitMode
{
	BLIT_MODE_NORMAL,
	BLIT_MODE_ROTATE_LEFT
};

void BlitBmp(int posX, int posY, byte *pDest, int dstX, int dstY, int dstPixelType, byte *pSrc, int srcX, int srcY, int srcPixelType, eBlitMode mode)
{
	assert(dstPixelType == GL_RGBA && srcPixelType == GL_RGBA);

	int dstBytesPerPix = 4;
	int dstPitch = dstX*dstBytesPerPix;

	//memset(pDest+dstPitch, 0, dstX* (dstY-10 )*4 );

	//return;

	byte *pDstCur = pDest+(  (posY+ (dstY-srcY)   )*dstPitch)+posX;

	for (int y=0; y < srcY; y++)
	{
		memcpy(pDstCur, pSrc, srcX*dstBytesPerPix);
		pSrc += srcX*dstBytesPerPix;
		pDstCur += dstPitch;
	}
}

void Surface::CopyFromScreen()
{
	int readX = GetPrimaryGLX();
	int readY = GetPrimaryGLY();

	int bytesPerPix = 4;

	byte * pBuff = new byte [readX*readY*bytesPerPix];

	//rotate it before we copy it
	int srcY = 0;

	glReadPixels(0, srcY, readX, readY, GL_RGBA, GL_UNSIGNED_BYTE, pBuff);
	CHECK_GL_ERROR();
	//now copy it to our texture
	assert(m_glTextureID != NO_TEXTURE_LOADED && "Texture can't be used yet");

	//BlitBmp(0,0, pFinal, m_texWidth, m_texHeight, GL_RGBA, pBuff, readX, readY, GL_RGBA, BLIT_MODE_ROTATE_LEFT);

	Bind();
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_texWidth, m_texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,  pFinal );
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, m_texHeight-readY, readX, readY, GL_RGBA, GL_UNSIGNED_BYTE, pBuff);
	CHECK_GL_ERROR();
	delete [] pBuff;
}


void Surface::FillColor(glColorBytes color)
{
	int width = GetWidth();
	int height = GetHeight();
	int bytesPerPixel = 4;

	byte *pBuf = new byte[width*height*bytesPerPixel];
	
	glColorBytes *pPixel = (glColorBytes*)pBuf;
	for (int i=0; i < width*height; i++)
	{
		*pPixel = color;
		pPixel++;
	}

	UpdateSurfaceRect(rtRect(0,0,width,height), pBuf);
	delete [] pBuf;
}

void Surface::UpdateSurfaceRect(rtRect dstRect, byte *pPixelData, bool bUpsideDownMode)
{
	Bind();
	int yStart = dstRect.top;
	
	if (bUpsideDownMode)
	{
		yStart += (m_texHeight-m_originalHeight);
	} 

	glTexSubImage2D(GL_TEXTURE_2D, 0, dstRect.left, yStart, dstRect.GetWidth(), dstRect.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);
	CHECK_GL_ERROR();
}

void Surface::SetTextureStates()
{

	switch(m_texType)
	{
	case TYPE_DEFAULT:
		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//	glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		break;

	case TYPE_GUI:

		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		/*
		#ifdef C_GL_MODE_
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //for gl 1.1 support
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//for gl 1.1 support

		#else
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//#endif		
		break;


	case TYPE_NO_SMOOTHING:

		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		/*
		#ifdef C_GL_MODE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //for gl 1.1 support
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//for gl 1.1 support

		#else
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//#endif	
		break;

	}
};



bool Surface::InitBlankSurface( int x, int y)
{
	Kill();

	assert(x && y);
	GLenum colorFormat = GL_RGBA;
	GLenum pixelFormat = GL_UNSIGNED_BYTE;
	int bytesPerPixel = 4;
	m_textureCreationMethod = TEXTURE_CREATION_BLANK;
	
	
	if (m_texWidth == 0)
	{
		//load manually, these kinds of surfaces are probably needed right away
		GetBaseApp()->m_sig_loadSurfaces.connect(1, boost::bind(&Surface::OnLoadSurfaces, this));

		//unload happens right away though
		GetBaseApp()->m_sig_unloadSurfaces.connect(1, boost::bind(&Surface::OnUnloadSurfaces, this));

	}
//	g_glesExt.extGlGenFramebuffers(1, &m_frameBuffer);
//	g_glesExt.extGlBindFramebuffer(GL_FRAMEBUFFER_OES, m_frameBuffer);

	m_originalWidth = x;
	m_originalHeight = y;

	m_texWidth =  nextPowerOfTwo(x);
	m_texHeight =  nextPowerOfTwo(y);
	
	//make a surface filled with 'unknown'

	PrepareGLForNewTexture();	
	
	m_bUsesAlpha = (colorFormat == GL_RGBA);
	
	byte *pPixelData = NULL;
	
	int dataSize = m_texWidth*m_texHeight*bytesPerPixel;

	bool bClearFirst = false;

//#ifdef _WIN32
	//the GL driver on the iPhone seems to start with a buffer of transparent pixels, on my windows box .. it's random
	//crap.  So we'll take the time to give it a default buffer of nothing first under win32.
	bClearFirst = true;
//#endif

if (bClearFirst)
{
	pPixelData = new byte[dataSize];
	assert(pPixelData);
	if (!pPixelData)
	{
		LogMsg("Low mem?");
		return false;
	}
	memset(pPixelData, 0, dataSize);
}

	int internalColorFormat = colorFormat;
#ifdef C_GL_MODE
	if (internalColorFormat == GL_RGBA) internalColorFormat = GL_RGBA8;
	if (internalColorFormat == GL_RGB) internalColorFormat = GL_RGB8;

#endif
	
	glTexImage2D( GL_TEXTURE_2D, 0, internalColorFormat, m_texWidth, m_texHeight, 0, colorFormat, pixelFormat, pPixelData );

	SAFE_DELETE_ARRAY(pPixelData);
	IncreaseMemCounter(dataSize);
	SetTextureStates();
	CHECK_GL_ERROR();

	/*
	g_glesExt.extGlFramebufferTexture2D(GL_FRAMEBUFFER_OES,
		GL_COLOR_ATTACHMENT0_OES,
		GL_TEXTURE_2D,
		m_glTextureID,
		0);
		*/

	CHECK_GL_ERROR();
	return true;
}

void Surface::ReloadImage()
{
	LoadFile(m_textureLoaded);
}

void Surface::OnLoadSurfaces()
{
	if (m_glTextureID != NO_TEXTURE_LOADED) return; //already loaded I guess


		switch (m_textureCreationMethod)
		{
		case TEXTURE_CREATION_FILE:

			#ifdef _DEBUG		
			LogMsg("Reloading texture %s", m_textureLoaded.c_str());
			#endif
			ReloadImage();
			break;

		case TEXTURE_CREATION_BLANK:
#ifdef _DEBUG		
			LogMsg("Recreating surface of %d, %d", m_originalWidth, m_originalHeight);
#endif
			InitBlankSurface(m_originalWidth, m_originalHeight);
			break;
		}
}
	

void Surface::OnUnloadSurfaces()
{

	if (m_glTextureID != NO_TEXTURE_LOADED)
	{
	#ifdef _DEBUG
		LogMsg("Unloading %d (%s)", m_glTextureID, m_textureLoaded.c_str());
	#endif
		Kill();
	}
}