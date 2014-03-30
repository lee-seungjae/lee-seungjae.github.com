#ifndef _UTF8_H_INCLUDED_
#define _UTF8_H_INCLUDED_
#pragma once

// #include <string>

// ��ȯ�� ���ڼ� ����
// ucs2�� ������ �ִ���̴� utf8count��ŭ�̴�. �˾Ƽ� ���� �Ҵ��ؼ� �Ѱ���� �Ѵ�.
// ��� ���ۿ� ���� ucs2 ���� ������ �����Ѵ�.
int  utf8_to_ucs2( wchar_t* ucs2, const char* utf8, int utf8count );

// utf8�� ������ �ִ���̴� ucs2*3��ŭ�̴�. �˾Ƽ� ���� �Ҵ��ؼ� �Ѱ���� �Ѵ�.
// ��� ���ۿ� ���� utf8 ����Ʈ ������ �����Ѵ�.
int  ucs2_to_utf8( const char* utf8, wchar_t* ucs2, int count );


std::wstring  utf8_to_ucs2( const std::string& utf8 );
std::string   ucs2_to_utf8( const std::wstring& ucs2 );

std::wstring  multibyte_to_ucs2( const std::string& multibyte, int codepage );
std::string   ucs2_to_multibyte( const std::wstring& ucs2, int codepage );

#endif