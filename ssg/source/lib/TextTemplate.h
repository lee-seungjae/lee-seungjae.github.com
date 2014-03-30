/////////////////////////////////////////////////////////////////////////////
// TextTemplate
//
// Author: Rica
// Date  : 2005/ 9/ 3
// 
// see http://upnl.org/wiki/wiki.php/Project/10secServ/TemplateEngine
//


#ifndef UPNL_TEXT_FORMAT_H_INCLUDED_
#define UPNL_TEXT_FORMAT_H_INCLUDED_
#pragma once

#pragma warning(disable:4786)
#pragma warning(disable:4503)
// #include <string>
// #include <vector>
// #include <map>
// #include <cassert>
// #include <cctype>
#include "OutputStream.h"

class REXI_Search;

namespace UPnL
{
	// == TextContent ==
	//  �����͸� ����־� �δ� �����̳�.
	//  ���⿡ �����͸� ���� �� OutputStream�� TextTemplate�� ������ �ȴ�.

	class TextContent
	{
	public:
		TextContent();
		virtual ~TextContent();

		typedef std::map<std::string, std::string>  StringTable;
		typedef std::vector<StringTable>            Array;
		typedef std::map<std::string, Array>        ArrayTable;

		// ����Ʈ�� ���� �Լ���
		void Clear();
		void AddString( const std::string& id, const std::string& text );
		void AddArray( const std::string& id, const Array& array );

		// ����Ʈ�� �д� �Լ���.
		// TextTemplate �ȿ��� �˾Ƽ� ����ϹǷ�, Ŭ������ ����ڴ� �Ű澵 �ʿ䰡 ����.
		bool ActivateArray( const std::string& id, int idx ) const;
		void DeactivateArray() const;
		int  GetArraySize( const std::string& id ) const;
		bool GetIdentifier( const std::string& id, std::string& result ) const;

	protected:
		StringTable  stringTab;
		ArrayTable   arrayTab;
		
		mutable const StringTable* activeArrayTabEntry;

		bool FindIdentifier( const std::string& id, const StringTable& tab, std::string& result ) const;
	};


	// == TextTemplate ==
	//  �̰��� �ѹ� �ε��� ������ ���� TextContent�� ���ؼ� ������ ���� �� �ִ�.

	class TextTemplate
	{
	public:
		TextTemplate();
		virtual ~TextTemplate();

		bool Load( const char* data, size_t size );
		void Print( const TextContent& tc, OutputStream& out ) const;

	private:
		TextTemplate( const TextTemplate& );
		void operator = ( const TextTemplate& );

	protected:
		class Scanner
		{
		public:
			Scanner( const char* buf, size_t size );
			virtual ~Scanner();
			
			enum Token
			{
				TOKEN_ID,
				TOKEN_IF,
				TOKEN_ELSE,
				TOKEN_END,
				TOKEN_FOREACH,
				TOKEN_TEXT,
				TOKEN_EOF,
				TOKEN_UTF8,
				TOKEN_QUOTED_UTF8,
			};

			enum EatOrNot
			{
				EAT,
				DONT_EAT
			};
			
			Token NextToken( std::string& str, EatOrNot eat );
			bool  NextTokenAsString( std::string& str );
			bool  Consume( char ch );	
			
		protected:
			std::vector<char>  text;

			int  idx;

			char LookAhead( int d )
			{
				assert(0<=idx+d && idx+d<(int)text.size());
				return text[idx+d];		
			}

			void Proceed( int d )
			{
				idx += d;
				assert(0<=idx && idx<=(int)text.size());
			}

			static bool IsIdentifierCharacter( char c )
			{
				return isalnum(c) || (c=='_');
			}

			static bool IsNormalCharacter( char c )
			{
				return (c!='$') && (c!='#') && (c!='\0');
			}

			std::string LookAheadAsString( int cnt )
			{
				std::string res;

				for( int i=0; i<cnt; i++ )
					res += LookAhead(i);

				return res;
			}

			char* GetCurrentBuffer()
			{
				return &text[idx];
			}

			bool IsEnd()
			{
				return idx >= (int)text.size();
			}
		};

	// === ���� ���� ���� ===
		class Block
		{
		public:
			Block();
			virtual ~Block();
			virtual void Print( const TextContent& tc, OutputStream& out ) const = 0;
		};

		class TextBlock : public Block
		{
		public:
			TextBlock( const std::string& text );
			virtual ~TextBlock();
			virtual void Print( const TextContent& tc, OutputStream& out ) const;

			std::string  text;
		};

		class ForBlock : public Block
		{
		public:
			ForBlock( const std::string& arrayName, Block* loop );
			virtual ~ForBlock();
			virtual void Print( const TextContent& tc, OutputStream& out ) const;

			std::string  arrayName;
			Block*       loop;
		};

		class IfBlock : public Block
		{
		public:
			IfBlock( std::string condition, Block* thenBlock, Block* elseBlock );
			virtual ~IfBlock();
			virtual void Print( const TextContent& tc, OutputStream& out ) const;

			std::string	 condition;
			Block*       thenBlock;
			Block*       elseBlock;
		};

		class CompoundBlock : public Block
		{
		public:
			CompoundBlock( const std::vector<Block*>& v );
			virtual ~CompoundBlock();
			virtual void Print( const TextContent& tc, OutputStream& out ) const;

			std::vector<Block*> contents;
		};

		class IdBlock : public Block
		{
		public:
			IdBlock( const std::string& text );
			virtual ~IdBlock();
			virtual void Print( const TextContent& tc, OutputStream& out ) const;

			std::string  id;
		};

		Block*  content;

// === �Ľ��� ������ �Լ��� ===
		Block*  ParseBlock( Scanner& scanner );
	};

}


#endif
