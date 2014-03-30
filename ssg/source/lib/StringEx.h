#ifndef UPNL_STRING_EX_H_INCLUDED_
#define UPNL_STRING_EX_H_INCLUDED_
#pragma once

// #include <string>
// ToString을 사용하고자 한다면 sstream을 인클루드할 것.
// StrToUpper, StrToLower를 사용하고자 한다면 algorithm을 인클루드할 것.

namespace UPnL
{
	// 온갖 잡다한 스트링 관련 조작을 이 모듈에 다 몰아넣는다.
	// 스트링 조작이라면 뭐든지 한다!!

	template< typename ISTREAM >
	void SkipSpace( ISTREAM& istr )
	{
		while ( isspace( istr.peek() ) )
			istr.get();
	}

	template< typename STR, typename CHARTYPE >
	void StrTrim_impl( STR& str, CHARTYPE* delim )
	{
		if( str.size() == 0 ) return;
		STR::size_type start = str.find_first_not_of(delim);
		STR::size_type end = str.find_last_not_of(delim);
		if( start == STR::npos )
		{
			str.clear();
			return;
		}
		else
		{
			assert( end != STR::npos );
			if( end < (str.size() - 1) ) str.erase( str.begin() + end + 1, str.end() );
			if( start > 0 ) str.erase( str.begin(), str.begin() + start );
		}
	}
	void inline StrTrim( std::string& str, char* delim="\r\n\t " )
	{
		StrTrim_impl(str, delim);
	}
	void inline StrTrim( std::wstring& str, wchar_t* delim=L"\r\n\t " )
	{
		StrTrim_impl(str, delim);
	}

	template< typename STR >
	void StrToUpper( STR& str )
	{
		std::transform( str.begin(), str.end(), str.begin(), ::toupper );
	}

	template< typename STR >
	void StrToLower( STR& str )
	{
		std::transform( str.begin(), str.end(), str.begin(), ::tolower );
	}
	
	template< typename STR >
	bool CompareWithoutCase( STR str1, STR str2 )
	{
		StrToUpper(str1);
		StrToUpper(str2);
		return str1 == str2;
	}

	template< typename STR >
	STR StrReplace( const STR& str, const STR& old_str, const STR& alt_str )
	{
		if( old_str.length()==0 ) return str;

		STR result(str);

		size_t found = result.find(old_str, 0);
		while( found != result.npos )
		{
			result.replace(found, old_str.length(), alt_str);
			found = result.find(old_str, found+alt_str.length());
		}
		return result;
	}

	template< typename STR >
	void Split( const STR& text, const STR& separators, std::vector<STR>& words )
    {
		STR::size_type n = text.length();

		STR::size_type start = text.find_first_not_of(separators);
		while( (start >= 0) && (start < n) )
		{
			STR::size_type stop = text.find_first_of(separators, start);
			if( (stop < 0) || (stop > n) )
				stop = n;
			words.push_back(text.substr(start, stop - start));
			start = text.find_first_not_of(separators, stop+1);
		}
    }
	
	template< typename STR >
	void Explode( const STR& text, const STR& seperators, std::vector<STR>& words )
	{
		if( text.empty() )
			return;

		typename STR::size_type n     = text.length();
		typename STR::size_type start = 0;
		while( start <= n )
		{
			typename STR::size_type stop = text.find_first_of(seperators, start);
			if( stop == STR::npos )
				stop = n;

			words.push_back(text.substr(start, stop - start));
			start = stop + 1;
		}
	}

	template< typename STR >
	STR Join( const std::vector<STR>& words, const STR& seperator )
	{
		// malformed implementation. by stania orz
		STR text;
		if( !words.empty() )
		{
			std::vector<STR>::const_iterator vit = words.begin();
			text += *(vit++);
			for( ; vit != words.end(); ++vit )
			{
				text += seperator;
				text += *vit;
			}
		}
		return text;
	}
	
	BOOL SetClipboardStr(LPCTSTR lpString);

	class StringLessIC
	{
	public:
		bool operator()( const std::string& lhs, const std::string& rhs ) const
		{
			return _stricmp(lhs.c_str(), rhs.c_str()) < 0;
		}
	};

	template< typename T >
	std::string ToString( T x )
	{
		ostringstream ostrstr;
		ostrstr << x;
		return ostrstr.str();
	}

	// 숫자를 세 문자 단위로 끊어서 콤마를 붙여 준다.
	template< typename CHARTYPE, typename INTTYPE >
	CHARTYPE* ItoaComma( CHARTYPE* const out, INTTYPE value )
	{
		CHARTYPE* p = out;

		if( value < 0 )
		{
			*p++ = '-';
			value  = -value;
		}
		
		INTTYPE divisor = 1;
		while( value/divisor >= 1000 )
		{
			divisor *= 1000;
		}
		
		INTTYPE v = value / divisor;
		if( v < 10 )  goto for1num;
		if( v < 100 ) goto for2nums;
	/*for3nums:*/
		*p++ = static_cast<CHARTYPE>('0' + v / 100);
	for2nums:
		*p++ = static_cast<CHARTYPE>('0' + v / 10 % 10);
	for1num:
		*p++ = static_cast<CHARTYPE>('0' + v % 10);

		divisor /= 1000;
		while( divisor > 0 )
		{
			v = (value / divisor) % 1000;
			*p++ = ',';
			*p++ = static_cast<CHARTYPE>('0' + v / 100);
			*p++ = static_cast<CHARTYPE>('0' + v / 10 % 10);
			*p++ = static_cast<CHARTYPE>('0' + v % 10);
			divisor /= 1000;
		}

		*p = 0;

		return out;
	}

    template< typename CHARTYPE >
	bool WildCardMatching( const CHARTYPE* pattern, const CHARTYPE* text )
	{
		if( *pattern==0 && *text==0 ) // 양쪽 문자열이 동시에 끝남
			return true;
		else if( *pattern==0 || *text==0 ) // 한쪽 문자열이 먼저 소진되어 버렸음
			return false;
		else if( *pattern == '*' ) // 와일드카드 매칭. 각각, 계속하는 경우와 끝난 경우.
			return WildCardMatching(pattern, text+1) || WildCardMatching(pattern+1, text+1);
		else if( *pattern == '?' || *pattern == *text ) // 한 문자 매칭
			return WildCardMatching(pattern+1, text+1);
		else
			return false;
	}

	std::string ToString( int bufSize, const char* fmt, ... );
	std::wstring ToStringW( int bufSize, const wchar_t* fmt, ... );
		// sprintf의 std::string 버전.

	std::string InsertComma( const std::string& number, size_t commaOffset = 3 );
		// 숫자를 commaOffset 단위로 끊어서 콤마를 삽입해 준다.

	std::string RawURLDecode( const std::string& in_req, bool verifyURL = true );
		// %xx인코딩된 문자열을 원래대로 돌려준다

	std::string URLDecode( const std::string& req, bool verifyURL = true );
		// 위와 같음. 그러나 '+' 를 ' ' 로 먼저 바꾼 후 번역한다.

	std::string URLEncode( const std::string& str );
		// ANSI코드로 표현되지 않는 문자,
		// 그리고 HTTP에서 특별한 의미를 갖는 문자들을 %xx로 표현한다.

	std::string GetFileName( const std::string& path );
		// 패스로부터 파일명을 얻는다.
		// 대소문자 변환은 하지 않는다.

	std::string GetExtension( const std::string& path );
		// 패스로부터 확장자를 얻는다.
		// 대소문자 변환은 하지 않는다.

	bool IsValidPath( const std::string & path );
		// 주어진 경로가 유효한지 검사한다.

	std::string AddNumberToFilename( std::string name, int num );
		// aaa.bbb 꼴의 파일 이름을 받아 aaa(num).bbb 꼴의 string을 만들어 리턴한다.
		// 문자열의 가장 앞의 . 앞에 삽입한다.( 파일명은 디렉토리 경로를 제거한 후 인자로 넘겨줘야한다. )

	bool IsBase64( const std::string &encodedString );
		// 문자열이 base64형태로 인코딩되었는지 확인해주는 함수.
	std::string DecodeBase64( const std::string& encodedString );
		// base64형태로 인코딩된 문자열을 받아서 원래 문자열을 리턴해주는 함수.
		// base64형태가 아닐 경우 빈 문자열을 리턴한다.

	unsigned long long ToUInt64( const std::string& num );

	template< typename CONTAINER >
    void XorTransform( CONTAINER& data, const CONTAINER& key )
	{
		CONTAINER::const_iterator ki = key.begin();
		CONTAINER::value_type x = *ki;
		for( CONTAINER::iterator di=data.begin(); di!=data.end(); ++di )
		{
			x    = static_cast<CONTAINER::value_type>(*ki * x);
			*di ^= x;

			++ki;
			if( ki == key.end() )
			{
				ki = key.begin();
			}
		}
	}

	bool starts_with(string str, string sub);

}

#endif
