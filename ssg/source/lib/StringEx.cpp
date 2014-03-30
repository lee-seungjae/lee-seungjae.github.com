#include "stdafx.h"
#include "StringEx.h"
#include "CodeConvert.h"
// #include <string>
// #include <algorithm>
// #include <functional>

using namespace std;

namespace UPnL
{
	std::wstring ToStringW( int bufSize, const wchar_t* fmt, ... )
	{
		wchar_t* buf = new wchar_t[bufSize];
		va_list ap;

		va_start(ap, fmt);
		vswprintf_s(buf, bufSize, fmt, ap);
		va_end(ap);

		std::wstring res( buf );
		delete[] buf;

		return res;
	}

	std::string ToString( int bufSize, const char* fmt, ... )
    {
		std::string res;

		char* buf = new char[bufSize];
		va_list ap;

		va_start(ap, fmt);
		vsprintf_s(buf, bufSize, fmt, ap);
		va_end(ap);

		res = buf;
		delete[] buf;

		return res;
    }
	
	std::string InsertComma( const std::string& number, size_t commaOffset )
	{
		string::const_iterator it = number.end();
		string toReturn;
		for( size_t i = commaOffset; i < number.size(); i += commaOffset )
		{
			toReturn = "," + string(it - commaOffset, it) + toReturn;
			it -= commaOffset;
		}
		if( it != number.begin() ) toReturn = string(number.begin(), it) + toReturn;
		else toReturn.erase(toReturn.begin(), toReturn.begin() + 1);

		return toReturn;
	}

	std::string RawURLDecode( const std::string& enc, bool verifyURL )
	{
		string result;
		result.reserve(enc.size());

		for( int i=0; i<(int)enc.size(); i++ )
		{
			if( i<(int)enc.size()-2 && enc[i]=='%' && isxdigit(enc[i+1]) && isxdigit(enc[i+2]) )
			{
				char buf[3];
				buf[0] = enc[++i];
				buf[1] = enc[++i];
				buf[2] = 0;

				int chr;
				sscanf_s(buf, "%x", &chr);

				result.push_back((char)chr);
			}
			else
			{
				result.push_back(enc[i]);
			}
		}

		// IIS Unicode Bug 보안 취약점 해결:
		// 00:29:59 <@i> < stania>  UTF sequence of C0XX may be decoded to ASCII XX and C1XX to ASCII
		// 00:30:00 <@i> < stania>  XX+0x40. E.G. '/' character may be encoded as C02F, and on some
		// 00:30:01 <@i> < stania>  systems also with C0AF and C19C. UTF sequences with more then 16 bits
		// 00:30:03 <@i> < stania>  are also can be decoded to valid ASCII character.
		if( verifyURL )
			return ucs2_to_utf8( utf8_to_ucs2( result ) );
		else
			return result;
	}

	std::string URLDecode( const std::string& req, bool verifyURL )
	{
		return RawURLDecode( StrReplace(req, string("+"), string(" ")), verifyURL );
	}

	std::string URLEncode( const std::string& str )
	{
		string result;
		result.reserve(str.length()*3);

		for( int i=0; i<(int)str.size(); i++ )
		{
			unsigned char c = (unsigned char)str[i];
			if( isalnum(c) || c=='_' || c=='.' || c=='/' )
			{
				result.push_back(c);
			}
			else
			{
				char buf[8];
				_snprintf_s(buf, sizeof(buf), "%02x", (int)c);
				result.push_back('%');
				result.push_back(buf[0]);
				result.push_back(buf[1]);
			}
		}

		return result;
	}

	string GetFileName( const string& p )
	{
		const string delim = "\\/";
		string::size_type endPos = p.find_last_not_of(delim);
		string::size_type beginPos= p.find_last_of(delim, endPos);
		beginPos = ( beginPos == string::npos )? 0: beginPos + 1;
		string::size_type len = (endPos == p.size()-1)? string::npos: endPos+1-beginPos; // string::substr 은 인자로 시작점, 부분문자열의 길이를 받는다.
		return p.substr(beginPos, len);
	}


	string GetExtension( const string& path )
	{
		// .이 존재하고 이 . 뒤에 /나 \\가 존재하지 않으면 .뒤의 문자열은 확장자이다.
		string::size_type dotPos = path.find_last_of(".");

		if( dotPos                  != string::npos &&
			path.find("/", dotPos)  == string::npos &&
			path.find("\\", dotPos) == string::npos )
		{
			return string(path.begin()+dotPos+1, path.end());
		}
		else
		{
			return string("");
		}
	}

	bool IsValidPath( const std::string & path )
	{
		// 아직 경로 중간에 ..를 포함하고 있는지만 검사한다.
		string::size_type parentPos = path.find("..");
		for( ; parentPos != string::npos; parentPos = path.find("..", parentPos + 2) )
		{
			if( parentPos == 0 ) continue;
			if( path.at(parentPos - 1) != '\\' && path.at(parentPos - 1) != '/' ) continue;
			if( path.size() == parentPos + 2 ) return false; // [\\/]..로 경로가 끝나는 경우
			if( path.at(parentPos + 2) == '\\' || path.at(parentPos + 2) == '/' ) return false;
		}

		return true;
	}

	BOOL SetClipboardStr(LPCTSTR lpString)
	{
		LPTSTR p;
		HGLOBAL hMem;
		HANDLE h;

		if(OpenClipboard(NULL))
		{
			hMem=GlobalAlloc(GHND, lstrlen(lpString)*sizeof(TCHAR)+sizeof(TCHAR));
			if(!hMem) return FALSE;

			p=(LPTSTR)GlobalLock(hMem);
			if ( p )
			{
				lstrcpy(p, lpString);
				GlobalUnlock(hMem);
			}
			else
			{
				GlobalFree(hMem);
				return FALSE;
			}

			EmptyClipboard();
			h = SetClipboardData(CF_UNICODETEXT, hMem);
#ifdef DEBUG
			if ( h == NULL )
			{
				LPVOID lpMsgBuf;
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					0, // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
				// Process any inserts in lpMsgBuf.
				// ...
				// Display the string.
				MessageBox( NULL, (LPCTSTR)lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION );
				// Free the buffer.
				LocalFree( lpMsgBuf );
			}
#endif

			CloseClipboard();
			GlobalFree(hMem);
			return TRUE;
		}

		return FALSE;
	}

	bool IsBase64( const std::string& encodedString )
	{
		if( encodedString.size() % 4 != 0 ) return false;

		for( int i = 0; i < (int)encodedString.size(); ++i )
		{
			if( (encodedString[i] >= '0') && (encodedString[i] <= '9') ) continue;
			if( (encodedString[i] >= 'A') && (encodedString[i] <= 'Z') ) continue;
			if( (encodedString[i] >= 'a') && (encodedString[i] <= 'z') ) continue;
			if( (encodedString[i] == '+') && (encodedString[i] == '/') ) continue; 
			if( (encodedString[i] != '=') || (i < (int)encodedString.size() - 2) ) return false;

			return (encodedString[encodedString.size() - 1] == '=');
		}

		return true;
	}

	// 이 함수는 외부에서 가져다 쓸 일이 없으므로 헤더에 선언하지 않는다.
	inline char LookupBase64Table( char c )
	{
		if( (c >= 'A') && (c <= 'Z') ) return c - 'A';
		if( (c >= 'a') && (c <= 'z') ) return c - 'a' + 26;
		if( (c >= '0') && (c <= '9') ) return c - '0' + 52;
		if( c == '+' ) return 62;
		if( c == '/' ) return 63;

		// not Base64
		return -1;
	}

	std::string DecodeBase64( const std::string& encodedString )
	{
		if( !IsBase64(encodedString) ) return "";

		char buf[4];
		string decoded;
		decoded.reserve(encodedString.size() / 4 * 3 + 1);

		int j = 0;
		for( int i = 0; i < (int)encodedString.size(); i += 4 )
		{
			int curr = 0;
			for( j = 0; j < 4; ++j )
			{
				buf[j] = LookupBase64Table(encodedString[i+j]);
				if( buf[j] < 0 ) break;
			}

			if( j < 4 ) break;

			for( j = 0, curr = 0; j < 4; ++j )
				curr |= buf[j] << (6 * (3 - j));

			for( j = 2; j >= 0; --j )
				decoded.push_back((char)((curr >> (j * 8)) & 0xFF));
		}

		int last = j;
		if( last > 1 )
		{
			int curr = 0;
			for( int i = 0; i < last; ++i )
				curr |= buf[i] << (6 * (3 - i));

			for( int i = 1; i >= 3 - last; --i )
				decoded.push_back((char)((curr >> ((i+1) * 8)) & 0xFF));
		}

		return decoded;
	}

	unsigned long long ToUInt64( const std::string& num )
	{
		string::size_type i = 0;
		for( ; (num[i] < '0' || num[i] > '9') && i < num.size() ; ++i ); 
		// trim의 역할. 앞부분부터 숫자가 아닌 문자는 무시.

		unsigned long long ret = 0;
		for( ; num[i] >= '0' && num[i] <= '9' && i < num.size(); ++i ) // 숫자가 아닌 문자가 나오면 정지.
		{
			ret = ret * 10 + num[i] - '0';
		}

		return ret;
	}

	bool starts_with(string str, string sub)
	{
		return str.substr(0, sub.length()) == sub;
	}


} // namespace UPnL