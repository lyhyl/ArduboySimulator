#include "PlatformPrecomp.h"
#include "MiscUtils.h"

//taken from Gamedeveloper magazine's InnerProduct (Sean Barrett 2005-03-15)

// circular shift hash -- produces good results if modding by a prime;
// longword at a time would be faster (need alpha-style "is any byte 0"),
// or just use the first longword

unsigned int HashString(const char *str)
{

	if (!str) return 0;

	unsigned char *n = (unsigned char *) str;
	unsigned int acc = 0x55555555;
	while (*n)
		acc = (acc >> 27) + (acc << 5) + *n++;
	return acc;
}

bool IsEven(int number)
{
	return (number & 1) == 0;
}


string BoolToEnabled(bool bEnabled)
{
	if (bEnabled) return "Enabled";
	return "Disabled";
}

string RemoveLastPartOfDir(string path)
{

	if (path.find_last_of("\\/") == string::npos)
	{
		//well, there is no path in this.. so the path is.. uh, blank I guess.
		return "";
	}
	if (path[path.size()-1] == '/' || path[path.size()-1] == '\\')
	{
		path = path.substr(0, path.find_last_of("\\/"));
	}
	path = path.substr(0, path.find_last_of("\\/")) + '\\';
	return path;
}

void ToLowerCase(char *pCharArray)
{
	for (int i=0; pCharArray[i]; i++)
	{
		pCharArray[i] = tolower(pCharArray[i]);
	}
}

void ToUpperCase(char *pCharArray)
{
	for (int i=0; pCharArray[i]; i++)
	{
		pCharArray[i] = toupper(pCharArray[i]);
	}
}

string ToLowerCaseString (const string & s)
{
	string d (s);
	for (unsigned int i=0; i < d.length(); i++)
	{
		d[i] = tolower(d[i]);
	}
	return d;
}  // end of tolower

string ToUpperCaseString (const string & s)
{
	string d (s);
	for (unsigned int i=0; i < d.length(); i++)
	{
		d[i] = toupper(d[i]);
	}
	return d;
}  // end of tolower

void SetIntWithTarget(int32 *p_out_dest, int r_target, int r_amount)
{
	if (*p_out_dest > r_target)
	{
		//make it less
		*p_out_dest -= r_amount;
		if (*p_out_dest < r_target) *p_out_dest = r_target;

	}   else
	{
		//make it more
		*p_out_dest += r_amount;
		if (*p_out_dest > r_target) *p_out_dest = r_target;
	}

}


void SetFloatWithTarget(float *p_out_dest, float r_target, float r_amount)
{
	if (*p_out_dest > r_target)
	{
		//make it less
		*p_out_dest -= r_amount;
		if (*p_out_dest < r_target) *p_out_dest = r_target;

	}   else
	{
		//make it more
		*p_out_dest += r_amount;
		if (*p_out_dest > r_target) *p_out_dest = r_target;
	}

}

int mod(int a, int b)
{

	assert(b != 0 && "You can't use 0 in this."); 
	int const rem = a % b;
	if ( (-1 % 2 == 1) || rem >= 0)
		return rem; else
		return rem + abs(b);
}

int Random(int range)
{
	return static_cast<int>(double(rand()) / (RAND_MAX) * range);
}

int RandomRange(int rangeMin, int rangeMax)
{
	return static_cast<int>(double(rand()) / RAND_MAX * (rangeMax-rangeMin)+rangeMin);
}

float RandomRangeFloat(float rangeMin, float rangeMax)
{
	return float(float(RandomRange(int(rangeMin*1000), int(rangeMax*1000))))/1000;
}

float LerpFloat(float f_origin, float f_target, float f_percent)
{
	return (f_origin - ((f_origin-f_target)*f_percent));
}

char *StringReverse(char *str)
{

	int length = strlen(str);
	// we need temp pointers to the beginning and
	// end of the sequence we wish to reverse
	char *start = str, *end = str + length - 1;
	while(start < end)
	{
		// swap characters, and move pointers towards
		// the middle of the sequence
		char temp = *start;
		*start++ = *end;
		*end-- = temp;
	}
	// return the reversed string
	return str;
}

string RemoveTrailingBackslash(string st)
{
	if (st.empty()) return st;

	while(st[st.size()-1] == '/' || st[st.size()-1] == '\\' )
	{
		st = st.substr(0, st.size()-1);
	}

	return st;
}

char *float_to_money( double num, char *buf, int dec)
{
      char tmp[256];
      int bf = 0, cm = 0, tm = 9 - dec + (!dec);

      sprintf(tmp, "%.9f", num);
      StringReverse(tmp);
      if(dec)
      {
            while( (buf[bf++] = tmp[tm++]) != '.')
                  ;
            while((buf[bf++] = tmp[tm++]) != 0)
            {
                  if(++cm % 3 == 0 && tmp[tm])
                        buf[bf++] = ',';
            }
      
      return StringReverse(buf);
      } else
      {
           //  while( (buf[bf++] = tmp[tm++]) != '.')
            //      ;
            while((buf[bf++] = tmp[tm++]) != 0)
            {
                  if(++cm % 3 == 0 && tmp[tm])
                        buf[bf++] = ',';
            }
      return StringReverse(buf);
   
      }
     return NULL;
}

string FloatToMoney(float f, int decimalsOfCents)
{
	char buf[256];
	float_to_money(double(f), buf, decimalsOfCents);
	return string(buf);
}


string PrefixLeading(const string input, unsigned int leadingCount, string leadingChar, string insertAfterPrefix)
{
	int charsNeeded = int(leadingCount)-input.size();
	if (charsNeeded < 0) return input;
	string s;
	while (charsNeeded--) s += leadingChar;

	s += insertAfterPrefix;

	return s+input;
}


string PostfixLeading(const string input, unsigned int leadingCount, string leadingChar, string insertAfterPrefix)
{
	if (input.length() > leadingCount) return input;
	
	string s = input;
	while (s.length() < leadingCount) s += leadingChar;
	return s + insertAfterPrefix;
}

vector<string> StringTokenize (const  string  & theString,  const  string  & theDelimiter )
{
	vector<string> theStringVector;

	size_t  start = 0, end = 0;

	while ( end != string::npos )
	{
		end = theString.find( theDelimiter, start );

		// If at end, use length=maxLength.  Else use length=end-start.
		theStringVector.push_back( theString.substr( start,
			(end == string::npos) ? string::npos : end - start ) );

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = (   ( end > (string::npos - theDelimiter.size()) )
			?  string::npos  :  end + theDelimiter.size()    );
	}

	return theStringVector;
}


bool in_range(int i_num, int i_low, int i_high)
{
	if (i_num > i_high) return false;
	if (i_num < i_low) return false;
	return true;
}

bool force_range(int * i_original, int i_min, int i_max)
{
	if (in_range(*i_original, i_min, i_max)) return false;
	if (*i_original < i_min) *i_original = i_min;
	if (*i_original > i_max) *i_original = i_max;
	return false;
}

void ForceRange(float &a, const float min, const float max)
{
	if (a < min) 
	{
		a = min;
		return;
	}
	
	if (a > max) 
	{
		a = max;
		return;
	}
}

string GetCountryCode() //return the 2 letter ISO country code, or 00 if we failed
{
	string country = "00";
	if (GetRegionString().length() == 5)
	{
		//looks right...
		country = ToLowerCaseString(GetRegionString().substr(3,2));
	}

	return country;
}

inline string TrimRight (const string & s, const string & t)
{ 
	string d (s); 
	string::size_type i (d.find_last_not_of (t));
	if (i == string::npos)
		return "";
	else
		return d.erase (d.find_last_not_of (t) + 1) ; 
}  // end of trim_right

string TrimLeft (const string & s, const string & t) 
{ 
	string d (s); 
	return d.erase (0, s.find_first_not_of (t)) ; 
}  // end of trim_left

string StripWhiteSpace(const string & s)
{
	string d (s); 
	return TrimLeft (TrimRight (d, RT_UTIL_SPACES), RT_UTIL_SPACES) ; 
}

string GetFileNameFromString(const string &path)
{

	for (int i=path.size()-1; i > 0; i--)
	{
		assert(path[i] != '\\' && "Do you want to handle these too?");

		if (path[i] == '/')
		{
			//well, this must be the cutoff point
			return path.substr(i+1, path.size()-i);
		}
	}
	return path;
}

string GetPathFromString(const string &path)
{

	for (int i=path.size()-1; i > 0; i--)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			//well, this must be the cutoff point
			return path.substr(0,i+1);
		}
	}
	return path;
}


string ModifyFileExtension(string fileName, string extension)
{
	size_t index = fileName.find_last_of('.');
	if (index == string::npos)
	{
		assert(!"Well, it doesn't have an extension to begin with");
		return fileName;
	}

	return fileName.substr(0, index+1) + extension;
}

string GetFileExtension(string fileName)
{
	size_t index = fileName.find_last_of('.');
	if (index == string::npos)
	{
		return "";
	}

	return fileName.substr(index+1, fileName.length());
}

void TruncateString(string &input, size_t len)
{
	if (input.length() > len)
	{
		input = input.substr(0, len);
	}
}

bool IsInString(string &s, const char *search)
{
	if (s.find(search) != string::npos) return true;
	return false;
}


void RotationToXYMod(float rotation, float *pXMod ,float *pYMod)
{
	switch (int(rotation))
	{
	case 0:

		*pXMod = 0; //no change
		*pYMod = 0; //no change
		return;
	case 90:
		*pXMod = 0; //no change
		*pYMod = 1; //change
		return;

	case 180:
		*pXMod = 1; //change!
		*pYMod = 1; //change
		return;

	case 270:
		*pXMod = 1; //chnage
		*pYMod = 0; //no change!
		return;

	}

	assert("Unknown degree");
	*pXMod = 0; //no change
	*pYMod = 0; //no change
	return;

}

