#include "StdAfx.h"
#include "TextTemplate.h"

#include "CodeConvert.h"

// #include <cassert>

using namespace std;

namespace UPnL
{
// == TextContent ==

	TextContent::TextContent()
	{
		activeArrayTabEntry = NULL;
	}

	TextContent::~TextContent()
	{
	}

	void TextContent::Clear()
	{
		stringTab.clear();
		arrayTab.clear();
		activeArrayTabEntry = NULL;
	}

	void TextContent::AddString( const string& id, const string& text )
	{
		stringTab[id] = text;
	}

	void TextContent::AddArray( const string& id, const Array& array )
	{
		arrayTab[id] = array;
	}

	bool TextContent::ActivateArray( const string& id, int idx ) const
	{
		ArrayTable::const_iterator it = arrayTab.find(id);
		if( it != arrayTab.end() )
		{
			activeArrayTabEntry = &it->second[idx];
			return true;
		}
		else
		{
			activeArrayTabEntry = NULL;
			return false;
		}
	}

	void TextContent::DeactivateArray() const
	{
		activeArrayTabEntry = NULL;
	}

	int TextContent::GetArraySize( const string& id ) const
	{
		ArrayTable::const_iterator it = arrayTab.find(id);
		if( it != arrayTab.end() )
		{
			return (int)it->second.size();
		}
		else
		{
			return 0;
		}		
	}

	bool TextContent::FindIdentifier( const string& id, const StringTable& tab, string& result ) const
	{
		StringTable::const_iterator it = tab.find(id);
		if( it != tab.end() )
		{
			result = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool TextContent::GetIdentifier( const string& id, string& result ) const
	{
		if( activeArrayTabEntry && FindIdentifier(id, *activeArrayTabEntry, result) )
			return true;

		if( FindIdentifier(id, stringTab, result) )
			return true;

		return false;
	}

	
// == TextTemplate ==

	TextTemplate::TextTemplate()
	{
		content = new TextBlock("");
	}

	TextTemplate::~TextTemplate()
	{
		assert(content);
		delete content;
	}

	bool TextTemplate::Load( const char* data, size_t size )
	{
		assert(content);

		try
		{
			Scanner scanner(data, size);

			Block* block = ParseBlock(scanner);
			assert(block);
			delete content;
			content = block;

			return true;
		}
		catch( const exception& ex )
		{
			printf("error: %s\n", ex.what());
			return false;
		}
		catch( ... )
		{
#ifdef NDEBUG
			return false;
#else
			throw;
#endif
		}
	}

	void TextTemplate::Print( const TextContent& tc, OutputStream& out ) const
	{
		assert(content);
		content->Print(tc, out);
	}

// === 파싱을 수행할 함수들 ===

	template< typename T >
	class auto_vector : public vector<T>
	{
	public:
		~auto_vector()
		{
			for( int i=0; i<(int)size(); i++ )
			{
				delete (*this)[i];
			}
		}
	};

	static void AssertParse( bool cond, char* what )
	{
		if( !cond )
		{
			throw exception(what);
		}
	}

	TextTemplate::Block* TextTemplate::ParseBlock( Scanner& scanner )
	{
		auto_vector<Block*> blocks;

		bool contLoop = true;
		while( contLoop )
		{
			string lexeme;
			int    token = scanner.NextToken(lexeme, Scanner::DONT_EAT);

			switch( token )
			{
				case Scanner::TOKEN_QUOTED_UTF8:
				case Scanner::TOKEN_UTF8:
					{
						// #U("왈라왈라")
						scanner.NextToken(lexeme, Scanner::EAT);
						AssertParse(
							scanner.Consume('('),
							"'(' expected after #U"
							);

						string untranslatedWord;
						AssertParse(
							scanner.NextTokenAsString(untranslatedWord),
							"\"Double-quoted string\" expected after #U("
							);

						AssertParse(
							scanner.Consume(')'),
							"')' expected after #U(\"...\""
							);

						//wstring translatedWord = 
						//	LocalizationStringTable::GetInstance().GetUCS2String(
						//		utf8_to_ucs2(untranslatedWord).c_str()
						//	);
						wstring translatedWord = utf8_to_ucs2(untranslatedWord);

						if (token == Scanner::TOKEN_QUOTED_UTF8)
							blocks.push_back(new TextBlock("\"" + ucs2_to_utf8(translatedWord) + "\""));
						else
							blocks.push_back(new TextBlock(ucs2_to_utf8(translatedWord)));
					}
					break;

				case Scanner::TOKEN_ID:
					{
						blocks.push_back(new IdBlock(lexeme));
						scanner.NextToken(lexeme, Scanner::EAT);
					}
					break;

				case Scanner::TOKEN_TEXT:
					{
						blocks.push_back(new TextBlock(lexeme));
						scanner.NextToken(lexeme, Scanner::EAT);
					}
					break;

				case Scanner::TOKEN_IF:
					{
						scanner.NextToken(lexeme, Scanner::EAT);

						string cond;
						AssertParse(
							scanner.Consume('('),
							"'(' expected after #if"
							);

						scanner.Consume(' ');
						AssertParse(
							scanner.NextToken(cond, Scanner::EAT) == Scanner::TOKEN_ID,
							"'$variable' expected in #if"
							);
						scanner.Consume(' ');

						AssertParse(
							scanner.Consume(')'),
							"')' expected after #if( $variable"
							);

						auto_ptr<Block> thenBlock(ParseBlock(scanner));
						auto_ptr<Block> elseBlock;

						int iftoken = scanner.NextToken(lexeme, Scanner::EAT);
						if( iftoken == Scanner::TOKEN_ELSE )
						{
							elseBlock.reset(ParseBlock(scanner));
							iftoken = scanner.NextToken(lexeme, Scanner::EAT);
						}

						AssertParse(
							iftoken == Scanner::TOKEN_END,
							"'#end' expected"
							);

						blocks.push_back(new IfBlock(cond, thenBlock.release(), elseBlock.release()));
					}
					break;
				
				case Scanner::TOKEN_FOREACH:
					{
						scanner.NextToken(lexeme, Scanner::EAT);

						string cond;
						AssertParse(
							scanner.Consume('('),
							"'(' expected after #foreach"
							);

						scanner.Consume(' ');
						AssertParse(
							scanner.NextToken(cond, Scanner::EAT) == Scanner::TOKEN_ID,
							"'$variable' expected in #foreach"
							);
						scanner.Consume(' ');

						AssertParse(
							scanner.Consume(')'),
							"')' expected after #foreach( $variable"
							);

						auto_ptr<Block> loop;
						
						loop.reset(ParseBlock(scanner));

						AssertParse(
							scanner.NextToken(lexeme, Scanner::EAT) == Scanner::TOKEN_END,
							"'#end' expected"
							);

						blocks.push_back(new ForBlock(cond, loop.release()));
					}
					break;

				case Scanner::TOKEN_EOF:
				case Scanner::TOKEN_ELSE:
				case Scanner::TOKEN_END:
					contLoop = false;					
					break;

				default:
					assert(false);
			};
		}

		Block* result;

		if( blocks.size() == 0 )
		{
			result = new TextBlock("");
		}
		else if( blocks.size() == 1 )
		{
			result = blocks[0];
		}
		else
		{
			result = new CompoundBlock(blocks);
		}

		blocks.clear();
		
		return result;
	}
	

// == Scanner ==
		
	TextTemplate::Scanner::Scanner( const char* buf, size_t size )
	{
		text.resize(size);
		memcpy(&text[0], buf, size);

		for( int i=0; i<10; i++ )
		{
			text.push_back((char)0);
		}

		idx = 0;
	}

	TextTemplate::Scanner::~Scanner()
	{
	}

	TextTemplate::Scanner::Token TextTemplate::Scanner::NextToken( std::string& str, EatOrNot eat )
	{
		char ch = LookAhead(0);

		// EOF
		if( ch == '\0' )
		{
			str = "";
			return TOKEN_EOF;
		}

		// Identifier
		if( ch == '$' && IsIdentifierCharacter(LookAhead(1)) )
		{
			int cnt = 1;
			while( IsIdentifierCharacter(LookAhead(cnt)) )
				cnt++;

			str.assign(GetCurrentBuffer(), cnt);
			if( eat == EAT )
				Proceed(cnt);

			return TOKEN_ID;
		}

		// Keywords
		if( ch == '#' )
		{
			str = "";
			const int   nKeyword = 6;
			const int   keywordLength[nKeyword] = {3, 5, 4, 8, 2, 3};
			const char* keywords[nKeyword]      = {"#if", "#else", "#end", "#foreach", "#U", "#QU"};
			const Token keywordType[nKeyword]   = {TOKEN_IF, TOKEN_ELSE, TOKEN_END, TOKEN_FOREACH, TOKEN_UTF8, TOKEN_QUOTED_UTF8};

			for( int i=0; i<nKeyword; i++ )
			{
				if( LookAheadAsString(keywordLength[i]) == keywords[i] )
				{
					if( eat == EAT )
						Proceed(keywordLength[i]);
					return keywordType[i];
				}
			}
		}

		// Text
		int cnt = 1;
		while( IsNormalCharacter(LookAhead(cnt)) )
			cnt++;

		str.assign(GetCurrentBuffer(), cnt);
		if( eat == EAT ) 
			Proceed(cnt);

		return TOKEN_TEXT;
	}

	bool TextTemplate::Scanner::NextTokenAsString( std::string& str )
	{
		// 시작 문자는 무조건 "다
		if( LookAhead(0) != '"' )
			return false;
		Proceed(1);

		// '\' 직후가 아닌 "를 찾으면 리턴한다.
        for( int i=0; !IsEnd(); i++ )
		{
			char c = LookAhead(i);
			if( c == '\\' )
			{
				i++;
				continue;
			}

            if( c == '"' )
			{
				str.assign(GetCurrentBuffer(), i);
				Proceed(i+1);
				return true;
			}
		}

		return false;
	}

	bool TextTemplate::Scanner::Consume( char ch )
	{
		while( isspace(LookAhead(0)) )
			Proceed(1);

		if( LookAhead(0) == ch )
		{
			Proceed(1);
			return true;
		}
		else
		{
			return false;
		}
	}


// == Block ==

	TextTemplate::Block::Block()
	{
	}

	TextTemplate::Block::~Block()
	{
	}


// == TextBlock ==

	TextTemplate::TextBlock::TextBlock( const string& text )
	{
		this->text = text;		
	}

	TextTemplate::TextBlock::~TextBlock()
	{
	}

	void TextTemplate::TextBlock::Print( const TextContent& tc, OutputStream& out ) const
	{
		(void)tc;
		out.Print(text);
	}


// == ForBlock ==

	TextTemplate::ForBlock::ForBlock( const string& arrayName, Block* loop )
	{
		this->arrayName = arrayName;
		this->loop      = loop;
	}

	TextTemplate::ForBlock::~ForBlock()
	{
		delete this->loop;
	}

	void TextTemplate::ForBlock::Print( const TextContent& tc, OutputStream& out ) const
	{
		int size = tc.GetArraySize(arrayName);

		for( int i=0; i<size; i++ )
		{
			tc.ActivateArray(arrayName, i);

			loop->Print(tc, out);
		}

		tc.DeactivateArray();
	}


// == IfBlock ==

	TextTemplate::IfBlock::IfBlock( std::string condition, Block* thenBlock, Block* elseBlock )
	{
		this->condition = condition;
		this->thenBlock = thenBlock;
		this->elseBlock = elseBlock;
	}

	TextTemplate::IfBlock::~IfBlock()
	{
		if( thenBlock )
			delete thenBlock;

		if( elseBlock )
			delete elseBlock;
	}

	void TextTemplate::IfBlock::Print( const TextContent& tc, OutputStream& out ) const
	{
		const Block* selectedBlock;
		string condValue;
		
		if( tc.GetArraySize(condition) ||
			tc.GetIdentifier(condition, condValue) && condValue.size() && condValue != "0" )
			selectedBlock = thenBlock;
		else
			selectedBlock = elseBlock;

		if( selectedBlock != NULL )
		{
			selectedBlock->Print(tc, out);
		}
	}

// == CompoundBlock ==

	TextTemplate::CompoundBlock::CompoundBlock( const vector<Block*>& v )
	{
		contents.assign(v.begin(), v.end());
	}

	TextTemplate::CompoundBlock::~CompoundBlock()
	{
		vector<Block*>::iterator it;
		for( it=contents.begin(); it!=contents.end(); ++it )
		{
			delete *it;
		}
	}

	void TextTemplate::CompoundBlock::Print( const TextContent& tc, OutputStream& out ) const
	{
		vector<Block*>::const_iterator it;
		for( it=contents.begin(); it!=contents.end(); ++it )
		{
			(*it)->Print(tc, out);
		}
	}

// == IdBlock ==

	TextTemplate::IdBlock::IdBlock( const string& id )
	{
		this->id = id;
	}

	TextTemplate::IdBlock::~IdBlock()
	{
	}

	void TextTemplate::IdBlock::Print( const TextContent& tc, OutputStream& out ) const
	{
		string content;
		if( tc.GetIdentifier(id, content) )
		{
			out.Print(content);
		}
		else
		{
			out.Print(id);
		}
	}

}
