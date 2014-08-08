#include "PlatformPrecomp.h"

#include "NetUtils.h"
#include "util/ResourceUtils.h"
#include "util/CRandom.h"
#include "util/MiscUtils.h"

//************ for base64 encoding, needed for http auth stuff *********


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/'};

static int mod_table[] = {0, 2, 1};


char *Base64Encode(const char *data, size_t input_length,size_t *output_length) 
{
	*output_length = (size_t) (4.0 * ceil((double) input_length / 3.0));

	char *encoded_data = (char*)malloc(*output_length);
	if (encoded_data == NULL) return NULL;

	for (uint32 i = 0, j = 0; i < input_length;) {

		uint32 octet_a = i < input_length ? data[i++] : 0;
		uint32 octet_b = i < input_length ? data[i++] : 0;
		uint32 octet_c = i < input_length ? data[i++] : 0;

		uint32 triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}

	for (int i = 0; i < mod_table[input_length % 3]; i++)
		encoded_data[*output_length - 1 - i] = '=';

	return encoded_data;
}
//*************************


/** web.h
* 1.Declares a class to encode strings converting a String 
* into a MIME format called "x-www-form-urlencoded" format. 
*	To convert a String, each character is examined in turn: 
*		1) The ASCII characters 'a' through 'z', 'A' through 'Z', and '0' through '9' remain the same. 
* 	2) The space character ' ' is converted into a plus sign '+'. 
*		3) All other characters are converted into the 3-character string "%xy", where xy is the two-digit hexadecimal representation of the lower 8-bits of the character. 
* 2.Declares a class to decode such strings
* 3. Declares the WebForm class to wrap win32 HTTP calls
* Author: Vijay Mathew Pandyalakal
* Date: 18/10/03

Stolen from CodeProject
**/


void DecToHexString ( uint32 value, byte * pOut, int16 charArrayMaxSize) 
{ 
	static byte  digit; 
	static int  i; 

	for (i = charArrayMaxSize - 1; i >= 0; i--) 
	{ 
		digit = byte((value & 0x0f) + 0x30);
		if (digit > 0x39) digit += 0x07; 
		pOut[i] = digit; 
		value >>= 4; 
	} 
} 


void URLEncoder::encodeData(const byte *pData, int len, string &finalReturn)
{
	char tmp[6];
	tmp[0] = '%';
	tmp[3] = 0;
	assert(len != 0 && len != -1 && "You need to send the length too.");
	string ret = "";

	vector<char> buff;
	buff.reserve(len*2);

	for(int i=0;i<len;i++) 
	{
		if(isOrdinaryChar((char)pData[i])) 
		{
			buff.push_back((char)pData[i]);
		}else if(pData[i] == ' ') 
		{
			buff.push_back('+');
		}else 
		{
			DecToHexString(pData[i], (byte*)&tmp[1], 2); //SETH, this should be faster than sprintf.  start after the % part
			//sprintf(tmp,"%%%02x",pData[i]); //SETH added 02 for leading zeros, serious bug fix.  Sent fix to author
			buff.push_back(tmp[0]);
			buff.push_back(tmp[1]);
			buff.push_back(tmp[2]);
		}
	}

	ret.reserve(ret.size()+buff.size()); //minimize mem allocations
	back_insert_iterator<string> dest (finalReturn);
	copy (buff.begin(), buff.end(), dest);
}


bool URLEncoder::isOrdinaryChar(char c) 
{
	if (
		(c >= 48 && c <= 57) ||
		(c >= 65 && c <= 90) ||
		(c >= 97 && c <= 122)) return true;
	return false;
}

string URLDecoder::decode(string str) 
{
	int len = str.length();
	char* buff = new char[len + 1];
	strcpy(buff,str.c_str());
	string ret = "";
	for(int i=0;i<len;i++) 
	{
		if(buff[i] == '+')
		{
			ret = ret + " ";
		}else if(buff[i] == '%')
		{
			char tmp[4];
			char hex[4];			
			hex[0] = buff[++i];
			hex[1] = buff[++i];
			hex[2] = '\0';		
			//int hex_i = atoi(hex);
			sprintf(tmp,"%c",convertToDec(hex));
			ret = ret + tmp;
		}else {
			ret = ret + buff[i];
		}
	}
	delete[] buff;
	return ret;
}

vector<byte> URLDecoder::decodeData(const string str) 
{
	int len = str.length();
	vector<byte> outBuff;
	outBuff.reserve(len);
	char hex[4];			
	hex[2] = '\0';		

	for(int i=0;i<len;i++) 
	{
		if(str[i] == '+')
		{
			outBuff.push_back(' ');
		}else if(str[i] == '%')
		{
			hex[0] = str[++i];
			hex[1] = str[++i];
			//int hex_i = atoi(hex);
			outBuff.push_back(byte(convertToDec(hex)));
		}  else 
		{
			outBuff.push_back(str[i]);
		}
	}

	return outBuff;
}
int URLDecoder::convertToDec(const char* hex)
{
	char buff[12];
	sprintf(buff,"%s",hex);
	int ret = 0;
	int len = strlen(buff);
	for(int i=0;i<len;i++) 
	{
		char tmp[4];
		tmp[0] = buff[i];
		tmp[1] = '\0';
		getAsDec(tmp);
		int tmp_i = atoi(tmp);
		int rs = 1;
		for(int j=i;j<(len-1);j++)
		{
			rs *= 16;
		}
		ret += (rs * tmp_i);
	}
	return ret;
}

void URLDecoder::getAsDec(char* hex) {
	char tmp = tolower(hex[0]);
	if(tmp == 'a') {
		strcpy(hex,"10");
	}else if(tmp == 'b') {
		strcpy(hex,"11");
	}else if(tmp == 'c') {
		strcpy(hex,"12");
	}else if(tmp == 'd') {
		strcpy(hex,"13");
	}else if(tmp == 'e') {
		strcpy(hex,"14");
	}else if(tmp == 'f') {
		strcpy(hex,"15");
	}else if(tmp == 'g') {
		strcpy(hex,"16");
	}
}



string GetDomainFromURL(string url)
{
	int pos = url.find("/");

	if (pos != string::npos)
	{
		return url.substr(0, pos);
	}

	return url;
}


//converts http://www.rtsoft.com/crap/crap.htm:81  to rtsoft.com, /crap/crap.htm, 81
void BreakDownURLIntoPieces(string url, string &domainOut, string &requestOut, int &port)
{
	port = 80;

	StringReplace("http://", "", url); //don't want that part

	if (url[0] == 'w' && url[1] == 'w' && url[2] == 'w'&& url[3] == '.')
	{
		StringReplace("www.", "", url); //don't want that part
	}

	int pos = url.find(":");
	if (pos != string::npos)
	{
		port = atol( url.substr(pos+1, url.size()- (pos+1)).c_str());
		url.erase(pos, url.size()-pos);
	}

	domainOut = GetDomainFromURL(url);

	requestOut = "";
	if (domainOut.empty()) return;
	if (url.size() == domainOut.size()) return; //something wrong
	requestOut = url.substr(domainOut.size()+1, url.size()- ( domainOut.size()+1 )  );
}



void GetSimpleGUID(uint32 *guid)
{
	int nowyear, nowmonth, nowday, nowhour, nowmin, nowsec;
	GetDateAndTime(&nowmonth, &nowday, &nowyear, &nowhour, &nowmin, &nowsec);

	CRandom r;


	guid[0] =  ((nowmonth+( (nowyear-2014)*12)) *(259200))+ (nowday*86400)+ (nowhour*3600)+nowsec;
	guid[1] = (uint32)Random(RT_RAND_MAX)*(uint32)Random(RT_RAND_MAX)+(uint32)Random(RT_RAND_MAX);

	r.SetRandomSeed(guid[0]+Random(RT_RAND_MAX)+nowyear);

	guid[2] = r.Random(200000000); // +Random(RT_RAND_MAX);
	guid[3] = (uint32)Random(RT_RAND_MAX)*(uint32)Random(RT_RAND_MAX)+(uint32)Random(RT_RAND_MAX);;
};

string GetSimpleGUIDAsString()
{

	uint32 guid[4];
	GetSimpleGUID((uint32*)&guid);

	char temp[32];

	string final;
	//convert to string
	for (int i=0; i < 4; i++)
	{
		memset(temp, 0, 32);
		DecToHexString(guid[i], (byte*)temp, 8);
		final += string(temp);
	}


	return final;
}