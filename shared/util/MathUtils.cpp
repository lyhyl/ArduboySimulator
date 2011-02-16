#include "PlatformPrecomp.h"
#include "MathUtils.h"

void ApplyOffset(CL_Rectf *pR, const CL_Vec2f &vOffset)
{
	pR->left += vOffset.x;
	pR->right += vOffset.x;

	pR->top += vOffset.y;
	pR->bottom += vOffset.y;
}

void ApplyPadding(CL_Rectf *pR, const CL_Vec2f &vPadding)
{
	pR->left -= vPadding.x;
	pR->right += vPadding.x;

	pR->top -= vPadding.y;
	pR->bottom += vPadding.y;
}

void ApplyPadding(CL_Rectf *pR, const CL_Rectf &vPadding)
{
	pR->left -= vPadding.left;
	pR->right += vPadding.right;

	pR->top -= vPadding.top;
	pR->bottom += vPadding.bottom;
}



CL_Rectf ScaleRect(const CL_Rectf &r, float scale)
{
	return CL_Rectf(r.left*scale, r.top*scale, r.right*scale,r.bottom*scale);
}

CL_Rectf ScaleRect2D(const CL_Rectf &r, CL_Vec2f vScale)
{
	return CL_Rectf(r.left*vScale.x, r.top*vScale.y, r.right*vScale.x,r.bottom*vScale.y);
}


string PrintVector2(CL_Vec2f v)
{
	char st[128];
	sprintf(st, "%.2f, %.2f", v.x, v.y);
	return string(st);
}

string PrintVector3(CL_Vec3f v)
{
	char st[128];
	sprintf(st, "%.3f, %.3f, %.3f", v.x, v.y, v.z);
	return string(st);
}

string PrintRect(CL_Rectf v)
{
	char st[128];
	sprintf(st, "%.3f, %.3f, %.3f, %.3f", v.left, v.top, v.right, v.bottom);
	return string(st);
}

string PrintRect(rtRectf v)
{
	char st[128];
	sprintf(st, "%.3f, %.3f, %.3f, %.3f", v.left, v.top, v.right, v.bottom);
	return string(st);
}
string PrintColor(uint32 color)
{
	char st[128];
	sprintf(st, "%d, %d, %d, %d", GET_RED(color), GET_GREEN(color), GET_BLUE(color), GET_ALPHA(color));
	return string(st);
}


CL_Vec2f GetAlignmentOffset(const CL_Vec2f &vSize, eAlignment alignment)
{
	switch (alignment)
	{

	case ALIGNMENT_UPPER_LEFT:
		return CL_Vec2f(0,0);

	case ALIGNMENT_UPPER_RIGHT:
		return CL_Vec2f(vSize.x,0);

	case ALIGNMENT_CENTER:
		return (vSize/2);
		break;
	case ALIGNMENT_DOWN_CENTER:
		return CL_Vec2f(vSize.x/2,vSize.y);
		break;
	case ALIGNMENT_UPPER_CENTER:
		return CL_Vec2f(vSize.x/2,0);
		break;
	case ALIGNMENT_DOWN_LEFT:
		return CL_Vec2f(0,vSize.y);
		break;
}

	assert(!"Unhandled alignment");
	return CL_Vec2f(0,0);
}


unsigned int DecryptPiece(byte *data, unsigned int size, int key)
{
	unsigned int i_checksum = 0;

	for (unsigned int i = 0; i < size; i++)
	{
		i_checksum += data[i]+key+i;
		data[i] = data[i]-(2+key+i);
	}

	return i_checksum;
}

#ifdef _DEBUG

#define C_EXTRA_DEBUG_CHECKS

#endif
unsigned int EncryptPiece(byte *data, unsigned int size, int key)
{

#ifdef C_EXTRA_DEBUG_CHECKS
	vector<byte> debugBuff;
	debugBuff.resize(size);
	memcpy(&debugBuff[0], data, size);
#endif

	unsigned int checksum = 0;

	for (unsigned int i = 0; i < size; i++)
	{
		data[i] = data[i]+2+key+i;
		checksum += data[i]+key+i;
	}

#ifdef C_EXTRA_DEBUG_CHECKS
	//make sure we can decrypt it accurately
	vector<byte> debugEncrypted;
	debugEncrypted.resize(size);
	memcpy(&debugEncrypted[0], data, size);

	//decrypt it
	int decryptChecksum = DecryptPiece(&debugEncrypted[0], size, key);

	//compare it to the original
	if (memcmp(&debugEncrypted[0], &debugBuff[0], size) != 0)
	{
		assert(!"Decrypted buffer doesn't match the original");
	}

	assert(decryptChecksum == checksum && "Checksums don't match");

#endif

	return checksum;
}


class rtCircle
{
public:
	rtCircle(CL_Vec2f vCenter, float radius) : c(vCenter), r(radius) {};

	CL_Vec2f c; //center position
	float r; //radius
};

bool IntersectRaySphere(CL_Vec2f p, CL_Vec2f d, rtCircle s, float &t, CL_Vec2f &q)
{

	CL_Vec2f m = p - s.c;
	float b = m.dot(d);
	float c = m.dot(m) - s.r * s.r;

	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
	if (c > 0.0f && b > 0.0f) return false;
	float discr = b*b - c;
	// A negative discriminant corresponds to ray missing sphere
	if (discr < 0.0f) return false;
	// Ray now found to intersect sphere, compute smallest t value of intersection
	t = -b - sqrt(discr);
	// If t is negative, ray started inside sphere so clamp t to zero
	if (t < 0.0f) t = 0.0f;
	q=p+(d*t);

	return true;
}


//circle : [C, r]
// segment [A, B]
// [P] : Point of collision on segment.

bool CircleSegmentIntersect(CL_Vec2f C, float r, CL_Vec2f A, CL_Vec2f B, CL_Vec2f& P)
{
	CL_Vec2f AC = C - A;
	CL_Vec2f AB = B - A;
	float ab2 = AB.dot(AB);
	float acab = AC.dot(AB);
	float t = acab / ab2;

	if (t < 0.0f) 
		t = 0.0f;
	else if (t > 1.0f) 
		t = 1.0f;

	P = A + AB * t;

	CL_Vec2f H = P - C;
	float h2 = H.dot(H);
	float r2 = r * r;

	if(h2 > r2) 
		return false;
	else
		return true;
}

//my MSVC2005 doesn't have round in cmath/math.h.  I'll just do it this so I won't break other builds...

inline double sethround(double val)
{   
	return floor(val + 0.5);
}


float ModNearestInt(float a, float b)
{
	return a - b * sethround(a / b);
}

/*
//let's you know how close two angles are to eachother.  Handles wrapping and such
bool AnglesAreClose(float a, float b, float angleTolerance)
{

	a = fmod(a, PI2);
	b = fmod(b, PI2);

	float mi = MIN(a, b);
	float mx = MAX(a, b);

	if (  !(( mx - mi) < angleTolerance) &&
		( !(  (mi + PI2) - mx < angleTolerance))) return false;

	return true;
}


*/