#ifndef _UTF8_H_INCLUDED_
#define _UTF8_H_INCLUDED_
#pragma once

// #include <string>

// 변환된 글자수 리턴
// ucs2의 가능한 최대길이는 utf8count만큼이다. 알아서 버퍼 할당해서 넘겨줘야 한다.
// 출력 버퍼에 쌓인 ucs2 워드 개수를 리턴한다.
int  utf8_to_ucs2( wchar_t* ucs2, const char* utf8, int utf8count );

// utf8의 가능한 최대길이는 ucs2*3만큼이다. 알아서 버퍼 할당해서 넘겨줘야 한다.
// 출력 버퍼에 쌓인 utf8 바이트 개수를 리턴한다.
int  ucs2_to_utf8( const char* utf8, wchar_t* ucs2, int count );


std::wstring  utf8_to_ucs2( const std::string& utf8 );
std::string   ucs2_to_utf8( const std::wstring& ucs2 );

std::wstring  multibyte_to_ucs2( const std::string& multibyte, int codepage );
std::string   ucs2_to_multibyte( const std::wstring& ucs2, int codepage );

#endif