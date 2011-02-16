
#ifndef MathUtils_h__
#define MathUtils_h__

#include "Math/rtRect.h"
#include "Math/rtPlane.h"
#include "CRandom.h"
#include "ClanLib-2.0/Sources/API/Core/Math/vec2.h"
#include "ClanLib-2.0/Sources/API/Core/Math/vec3.h"
#include "ClanLib-2.0/Sources/API/Core/Math/rect.h"

#include "ClanLib-2.0/Sources/API/Core/Math/mat3.h"
#include "ClanLib-2.0/Sources/API/Core/Math/mat4.h"
#include "ClanLib-2.0/Sources/api/Core/Math/line_segment.h"
#include "ClanLib-2.0/Sources/api/Core/Math/circle.h"

using namespace std;

string PrintVector2(CL_Vec2f v);
string PrintVector3(CL_Vec3f v);
string PrintRect(CL_Rectf v);
string PrintRect(rtRectf v);

string PrintColor(uint32 color);

CL_Vec2f GetAlignmentOffset(const CL_Vec2f &vSize, eAlignment alignment);
void ApplyPadding(CL_Rectf *pR, const CL_Vec2f &vPadding);
void ApplyPadding(CL_Rectf *pR, const CL_Rectf &vPadding);
void ApplyOffset(CL_Rectf *pR, const CL_Vec2f &vOffset);
unsigned int EncryptPiece(byte *data, unsigned int size, int key);
unsigned int DecryptPiece(byte *data, unsigned int size, int key);
CL_Rectf ScaleRect(const CL_Rectf &r, float scale);
CL_Rectf ScaleRect2D(const CL_Rectf &r, CL_Vec2f vScale);

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x))) //thanks to sol_hsa at http://sol.gfxile.net/interpolation/index.html
#define EASE_TO(x) (1 - (1 - x) * (1 - x))
#define EASE_FROM(x) (x*x)
#define SMOOTHSTEP_INVERSE(x) pow( (x/0.5)-1,3)

bool CircleSegmentIntersect(CL_Vec2f C, float r, CL_Vec2f A, CL_Vec2f B, CL_Vec2f& P);
float ModNearestInt(float a, float b);
bool AnglesAreClose(float a, float b, float angleTolerance);
#endif // MathUtils_h__
