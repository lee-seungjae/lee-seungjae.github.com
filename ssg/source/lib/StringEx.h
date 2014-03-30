#ifndef UPNL_STRING_EX_H_INCLUDED_
#define UPNL_STRING_EX_H_INCLUDED_
#pragma once

// #include <string>
// ToString�� ����ϰ��� �Ѵٸ� sstream�� ��Ŭ����� ��.
// StrToUpper, StrToLower�� ����ϰ��� �Ѵٸ� algorithm�� ��Ŭ����� ��.

namespace UPnL
{
	// �°� ����� ��Ʈ�� ���� ������ �� ��⿡ �� ���Ƴִ´�.
	// ��Ʈ�� �����̶�� ������ �Ѵ�!!

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

	// ���ڸ� �� ���� ������ ��� �޸��� �ٿ� �ش�.
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
		if( *pattern==0 && *text==0 ) // ���� ���ڿ��� ���ÿ� ����
			return true;
		else if( *pattern==0 || *text==0 ) // ���� ���ڿ��� ���� �����Ǿ� ������
			return false;
		else if( *pattern == '*' ) // ���ϵ�ī�� ��Ī. ����, ����ϴ� ���� ���� ���.
			return WildCardMatching(pattern, text+1) || WildCardMatching(pattern+1, text+1);
		else if( *pattern == '?' || *pattern == *text ) // �� ���� ��Ī
			return WildCardMatching(pattern+1, text+1);
		else
			return false;
	}

	std::string ToString( int bufSize, const char* fmt, ... );
	std::wstring ToStringW( int bufSize, const wchar_t* fmt, ... );
		// sprintf�� std::string ����.

	std::string InsertComma( const std::string& number, size_t commaOffset = 3 );
		// ���ڸ� commaOffset ������ ��� �޸��� ������ �ش�.

	std::string RawURLDecode( const std::string& in_req, bool verifyURL = true );
		// %xx���ڵ��� ���ڿ��� ������� �����ش�

	std::string URLDecode( const std::string& req, bool verifyURL = true );
		// ���� ����. �׷��� '+' �� ' ' �� ���� �ٲ� �� �����Ѵ�.

	std::string URLEncode( const std::string& str );
		// ANSI�ڵ�� ǥ������ �ʴ� ����,
		// �׸��� HTTP���� Ư���� �ǹ̸� ���� ���ڵ��� %xx�� ǥ���Ѵ�.

	std::string GetFileName( const std::string& path );
		// �н��κ��� ���ϸ��� ��´�.
		// ��ҹ��� ��ȯ�� ���� �ʴ´�.

	std::string GetExtension( const std::string& path );
		// �н��κ��� Ȯ���ڸ� ��´�.
		// ��ҹ��� ��ȯ�� ���� �ʴ´�.

	bool IsValidPath( const std::string & path );
		// �־��� ��ΰ� ��ȿ���� �˻��Ѵ�.

	std::string AddNumberToFilename( std::string name, int num );
		// aaa.bbb ���� ���� �̸��� �޾� aaa(num).bbb ���� string�� ����� �����Ѵ�.
		// ���ڿ��� ���� ���� . �տ� �����Ѵ�.( ���ϸ��� ���丮 ��θ� ������ �� ���ڷ� �Ѱ�����Ѵ�. )

	bool IsBase64( const std::string &encodedString );
		// ���ڿ��� base64���·� ���ڵ��Ǿ����� Ȯ�����ִ� �Լ�.
	std::string DecodeBase64( const std::string& encodedString );
		// base64���·� ���ڵ��� ���ڿ��� �޾Ƽ� ���� ���ڿ��� �������ִ� �Լ�.
		// base64���°� �ƴ� ��� �� ���ڿ��� �����Ѵ�.

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
