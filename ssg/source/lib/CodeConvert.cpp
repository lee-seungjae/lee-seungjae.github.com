#include "stdafx.h"
#include "CodeConvert.h"

// #include <string>
// #include <vector>
// #include <wchar.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std;

int utf8_to_ucs2( wchar_t* ucs2, const char* utf8, int utf8size )
{
	int count = 0;
	const char* limit = utf8 + utf8size;

	while( utf8 < limit )
	{
		if( (utf8[0]&0x80) == 0 )			// 1 byte 
		{
			*ucs2 = utf8[0]; 
			utf8++;
		}
		else if( (utf8[0]&0xE0) == 0xc0 )	// 2 bytes 
		{
			if( utf8 + 1 >= limit ) break;
			*ucs2 = ((utf8[0] & 0x1f) << 6) | (utf8[1] & 0x3f); 
			utf8 += 2;
		}
		else if( (utf8[0]&0xf0) == 0xe0 )	// 3 bytes
		{
			if( utf8 + 2 >= limit ) break;
			*ucs2 = ((utf8[0] & 0x0f) << 12) | ((utf8[1] & 0x3f) << 6) | (utf8[2] & 0x3f);
			utf8 += 3;
		}
		else 
		{
			*ucs2 = L'?';
			utf8 += 1;
		}

		ucs2++;
		count++;
	}

	return count;
}


int ucs2_to_utf8( char* utf8, const wchar_t* ucs2buf, int ucs2size )
{
	int count = 0;
	const wchar_t* limit = ucs2buf + ucs2size;

	while( ucs2buf < limit )
	{
		wchar_t ucs2 = *ucs2buf;

		if( (ucs2 & 0xFF80) == 0 )		// 1 byte 
		{
			utf8[0] = (byte)ucs2;
			utf8  += 1;
			count += 1;
		}
		else if( (ucs2 & 0xf800) == 0 )	// 2 bytes 
		{
			utf8[0] = (byte)(0xc0|(ucs2>>6)); 
			utf8[1] = (byte)(0x80|(ucs2&0x3f));
			utf8  += 2;
			count += 2;
		} 
		else							// 3 bytes 
		{
			utf8[0] = (byte)(0xe0|(ucs2>>12));
			utf8[1] = (byte)(0x80|((ucs2>>6)&0x3f));
			utf8[2] = (byte)(0x80|(ucs2&0x3f));
			utf8  += 3;
			count += 3;
		}

		ucs2buf++;
	}

	return count;
}


std::wstring utf8_to_ucs2( const std::string& utf8 )
{
	vector<wchar_t> ucs2;

	ucs2.resize(utf8.size()+5);
	size_t ucs2Count = utf8_to_ucs2(&ucs2[0], utf8.c_str(), (int)utf8.size());

	ucs2[ucs2Count] = '\0';

	return wstring(&ucs2[0]);
}

std::string ucs2_to_utf8( const std::wstring& ucs2 )
{
	vector<char> utf8;

	utf8.resize(ucs2.size()*3 + 5);
	size_t utf8Count = ucs2_to_utf8(&utf8[0], ucs2.c_str(), (int)ucs2.size());

	utf8[utf8Count] = '\0';

	return string(&utf8[0]);
}

std::string ucs2_to_multibyte( const std::wstring& ucs2, int codepage )
{
	int ucs2Count = (int)ucs2.size();

	vector<char> multibyte;
	multibyte.resize(ucs2Count*2 + 5);

	BOOL defCharUsed;
	int converted = WideCharToMultiByte(
		codepage,
		0,
		&ucs2[0],
		ucs2Count,
		&multibyte[0],
		(int)multibyte.size(),
		"_",
		&defCharUsed
		);

	multibyte[converted] = '\0';

	return string(&multibyte[0]);
}

std::wstring multibyte_to_ucs2( const std::string& multibyte, int codepage )
{
	vector<wchar_t> ucs2;
	ucs2.resize(multibyte.size() + 5);
	int converted = MultiByteToWideChar(
		codepage,
		0,
		multibyte.c_str(),
		(int)multibyte.size(),
		&ucs2[0], 
		(int)ucs2.size()
		);

	ucs2[converted] = '\0';

	return wstring(&ucs2[0]);
}
