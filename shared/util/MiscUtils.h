#pragma once
#include <string>

unsigned int HashString(const char *str);
bool IsEven(int number);
void ToLowerCase(char *pCharArray);
void ToUpperCase(char *pCharArray);
string ToLowerCaseString (const string & s);
string ToUpperCaseString (const string & s);

void SetIntWithTarget(int32 *p_out_dest, int r_target, int r_amount);
int mod(int a, int b);
int Random(int range);
int RandomRange(int rangeMin, int rangeMax);
float RandomRangeFloat(float rangeMin, float rangeMax);
float LerpFloat(float f_origin, float f_target, float f_percent);
string RemoveLastPartOfDir(string path);
string BoolToEnabled(bool bEnabled);
char *float_to_money( double num, char *buf, int dec); //for legacy code..
string FloatToMoney(float f, int decimalsOfCents = 0);
vector<string> StringTokenize(const string& str,const string& delimiters, const string & secondOptionalDelimiter);
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
string PrefixLeading(const string input, unsigned int leadingCount, string leadingChar, string insertAfterPrefix = "");
string PostfixLeading(const string input, unsigned int leadingCount, string leadingChar, string insertAfterPrefix = "");
vector<string> StringTokenize(const string& str,const string& delimiters);
bool force_range(int * i_original, int i_min, int i_max);
string GetCountryCode(); //return the 2 letter ISO country code, or 00 if we failed
string StripWhiteSpace(const string &s);
#define RT_UTIL_SPACES " \t\r\n"
string TrimLeft (const string & s, const string & t = RT_UTIL_SPACES);
string TrimRight (const string & s, const string & t = RT_UTIL_SPACES);
string GetFileNameFromString(const string &path);
string GetPathFromString(const string &path);
string RemoveTrailingBackslash(string st);
string GetFileExtension(string fileName);
string ModifyFileExtension(string fileName, string extension);
void TruncateString(string &input, size_t len);
bool IsInString(string &s, const char *search);
void RotationToXYMod(float rotation, float *pXMod ,float *pYMod); //given a rotation in degrees, tells you what to flip
void SetFloatWithTarget(float *p_out_dest, float r_target, float r_amount);
string FilterToValidAscii(const string &input, bool bStrict);
bool isOrdinaryChar(char c, bool bStrict);