#ifndef UPNL_OUTPUT_STREAM_H_INCLUDED_
#define UPNL_OUTPUT_STREAM_H_INCLUDED_
#pragma once

// #include <string>

namespace UPnL
{
	// == OutputStream ==
	//  �̰��� Output�� ����ؼ�, TextTemplate�� ������� ���� �̾Ƴ��� Ŀ���͸������Ѵ�.

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