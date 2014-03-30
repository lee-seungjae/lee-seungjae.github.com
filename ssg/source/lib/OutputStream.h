#ifndef UPNL_OUTPUT_STREAM_H_INCLUDED_
#define UPNL_OUTPUT_STREAM_H_INCLUDED_
#pragma once

// #include <string>

namespace UPnL
{
	// == OutputStream ==
	//  이곳의 Output을 상속해서, TextTemplate의 결과물을 어디로 뽑아낼지 커스터마이즈한다.

	class OutputStream
	{
	public:
		OutputStream()
		{
		}
		virtual ~OutputStream()
		{
		}

		void Print( const std::string& str )
		{
			Output(str.c_str(), (int)str.length());
		}
		void Print( const char* str )
		{
			Output(str, (int)strlen(str));
		}

		virtual void Output( const char* text, int size ) = 0;
	};
}

#endif