#ifndef MiscUtils_h__
#define MiscUtils_h__

#include <string>

#include "PlatformSetup.h"

unsigned int HashString(const char *str);
bool IsEven(int number);
void ToLowerCase(char *pCharArray);
void ToUpperCase(char *pCharArray);
std::string ToLowerCaseString (const std::string & s);
std::string ToUpperCaseString (const std::string & s);

bool CaseInsensitiveCompare(const char*a, const char*b);
void SetIntWithTarget(int32 *p_out_dest, int r_target, int r_amount);
int mod(int a, int b);
int Random(int range);
int RandomRange(int rangeMin, int rangeMax);
float RandomRangeFloat(float rangeMin, float rangeMax);
float LerpFloat(float f_origin, float f_target, float f_percent);
std::string RemoveLastPartOfDir(std::string path);
std::string BoolToEnabled(bool bEnabled);
char *float_to_money( double num, char *buf, int dec); //for legacy code..
std::string FloatToMoney(float f, int decimalsOfCents = 0);

std::vector<std::string> StringTokenize(const std::string& str,const std::string& delimiters);
string PopFirstParmString(string *lineInOut, const string delimiter = "|"); //removes the first parm from the string sent in, and returns it

//helper to turn anything into a string, like ints/floats
void ForceRange(float &a, const float min, const float max);

#define rt_min(a,b)    (((a) < (b)) ? (a) : (b))
#define rt_max(a,b)            (((a) > (b)) ? (a) : (b))

template< class C>
std::string toString(C value)
{
	std::ostringstream o;
	o << value;
	return o.str();
}
std::string PrefixLeading(const std::string input, unsigned int leadingCount, std::string leadingChar, std::string insertAfterPrefix = "");
std::string PostfixLeading(const std::string input, unsigned int leadingCount, std::string leadingChar, std::string insertAfterPrefix = "");

bool force_range(int * i_original, int i_min, int i_max);
std::string GetCountryCode(); //return the 2 letter ISO country code, or 00 if we failed
std::string StripWhiteSpace(const std::string &s);
#define RT_UTIL_SPACES " \t\r\n"
std::string TrimLeft (const std::string & s, const std::string & t = RT_UTIL_SPACES);
std::string TrimRight (const std::string & s, const std::string & t = RT_UTIL_SPACES);
std::string GetFileNameFromString(const std::string &path);
std::string GetPathFromString(const std::string &path);
std::string RemoveTrailingBackslash(std::string st);
std::string GetFileExtension(std::string fileName);
std::string ModifyFileExtension(std::string fileName, std::string extension);
void TruncateString(std::string &input, size_t len);
bool IsInString(std::string &s, const char *search);
void RotationToXYMod(float rotation, float *pXMod ,float *pYMod); //given a rotation in degrees, tells you what to flip
void SetFloatWithTarget(float *p_out_dest, float r_target, float r_amount);
std::string FilterToValidAscii(const std::string &input, bool bStrict);
bool isOrdinaryChar(char c, bool bStrict);
#endif
