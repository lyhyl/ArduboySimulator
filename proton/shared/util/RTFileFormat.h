#ifndef RTFileFormat_h__
#define RTFileFormat_h__

//all RT file formats start with the same main header...

struct RTFileHeader
{
	char fileTypeID[6]; 
	byte version;
	byte reserved[1];
};


enum eCompressionType
{
	C_COMPRESSION_NONE = 0,
	C_COMPRESSION_ZLIB = 1
};

#define C_RTFILE_PACKAGE_LATEST_VERSION 0
#define C_RTFILE_PACKAGE_HEADER "RTPACK"

struct rtpack_header
{
	RTFileHeader rtFileHeader;
	unsigned int compressedSize;
	unsigned int decompressedSize;
	byte compressionType; //one of eCompressionType
	byte reserved[15];
};


#define C_RTFILE_TEXTURE_HEADER "RTTXTR"
struct rttex_header
{
	RTFileHeader rtFileHeader;

	//our custom header
	int height;
	int width;
	int format; // probably GL_COMPRESSED_RGBA_ATI_TC
	int originalHeight; //before we padded to be a power of 2, if applicable.
	int originalWidth; //before we padded to be a power of 2, if applicable.
	unsigned char bUsesAlpha;
	unsigned char reservedFlags[3]; //keep it packed right
	int mipmapCount; //how many tex infos are followed...

	int reserved[16];

	//texture info to follow, 1 for each mip map
};


struct rttex_mip_header
{
	int height;
	int width;
	int dataSize;
	int mipLevel;
	int reserved[2];
};



#endif // RTFileFormat_h__
