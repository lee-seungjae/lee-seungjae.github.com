// ssg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lib/TextTemplate.h"
#include "lib/file_iterator.h"
#include "lib/CodeConvert.h"
#include "lib/FileOutputStream.h"
#include "lib/StringEx.h"

#include <set>
#include <fstream>
#include <iostream>
#include <algorithm>

set<wstring> docNames;
wstring textDir;// = L"text/"; # config에서 읽음
wstring textExt = L".txt";
wstring htmlDir;// = L"html/"; # config에서 읽음
wstring htmlExt = L".html";
string extHtml8 = ".html";
wstring skinFile; // # config에서 읽음

class LinkProvider
{
public:
	virtual ~LinkProvider() {}
	virtual string GetTitle(string context, string docID) const = 0;
};

class DummyLinkProvider : public LinkProvider
{
	virtual string GetTitle(string context, string docID) const override { return "?"; }
};

class DocumentList : public LinkProvider
{
public:
	void Add(string docID, string title, string firstDate)
	{
		Summary s;
		s.title = title;
		s.firstDate = firstDate;
		docs[docID] = s;
	}

	virtual string GetTitle(string context, string docID) const override
	{
		auto it = docs.find(docID);
		if (it != docs.end())
		{
			return it->second.title;
		}
		wcerr << L"[" << utf8_to_ucs2(context)
			<< L"] WARNING: 링크 대상 문서가 존재하지 않음 ["
			<< utf8_to_ucs2(docID) << L"]" << endl;
		return docID;
	}

	void ExportTo(TextContent& tc)
	{
		vector<pair<string, Summary>> vdocs(docs.begin(), docs.end());
		struct CompareByFirstDate
		{
			bool operator()(const pair<string, Summary>& lhs, const pair<string, Summary>& rhs) const
			{
				return lhs.second.firstDate > rhs.second.firstDate;
			}
		};
		sort(vdocs.begin(), vdocs.end(), CompareByFirstDate());

		TextContent::Array arr;
		for (auto it = vdocs.begin(); it != vdocs.end(); ++it)
		{
			TextContent::StringTable t;
			t["$link"] = it->first + ucs2_to_utf8(htmlExt);
			t["$title"] = it->second.title;
			t["$date"] = it->second.firstDate;
			arr.push_back(t);
		}
		tc.AddArray("$index", arr);
	}

	struct Summary
	{
		string title;
		string firstDate;
	};

	map<string, Summary> docs; // id -> summary
};

///////////////////////////////////////////////////////////////////////////////
class Parser
{
public:
	//-------------------------------------------------------------------------
	Parser(wstring docID, const LinkProvider& linkProvider, DocumentList& docList)
		: docID(docID)
		, fin((textDir + docID + textExt).c_str(), fstream::in)
		, linkProvider(linkProvider)
		, docList(docList)
	{
		if (!fin.is_open()) { Error(L"파일 열지 못함", ""); }

		// BOM
		int firstByte = fin.get();
		if (firstByte == 0xEF)
		{
			fin.get();
			fin.get();
		}
		else
		{
			fin.putback(firstByte);
		}
	}

	//-------------------------------------------------------------------------
	void ParseHeader()
	{
		getline(fin, title);
		if (title.substr(0, 2) != "# ")
		{
			Error(L"첫 줄은 '# ' 로 시작해야 합니다.", title);
		}
		title = title.substr(2);
		
		SkipEmptyLine();

		int c = fin.get();
		fin.putback(c);
		while (c == '*')
		{
			string ln;
			getline(fin, ln);

			int success =
				TryRead(ln, firstDate, L"작성") ||
				TryRead(ln, lastDate, L"최종수정") ||
				TryRead(ln, tags, L"태그");
			if (!success)
			{
				Error(L"헤더 파싱 실패: ", ln);
			}

			c = fin.get();
			fin.putback(c);
		}
		SkipEmptyLine();

		docList.Add(ucs2_to_utf8(docID), title, firstDate);
	}

	//-------------------------------------------------------------------------
	bool TryRead(string ln, string& var, wstring hname)
	{
		string head = "* " + ucs2_to_utf8(hname) + ": ";
		if (starts_with(ln, head))
		{
			var = ln.substr(head.length());
			return true;
		}
		return false;
	}

	//-------------------------------------------------------------------------
	void ParseBody()
	{
		parseBodyFunc = &Parser::ParseBody_Normal;
		while (!fin.eof())
		{
			string ln;
			getline(fin, ln);
			body += (this->*parseBodyFunc)(ln);

//			wcout << utf8_to_ucs2(ln) << endl;
		}
	}
	string (Parser::*parseBodyFunc)(const string& ln);

	//-------------------------------------------------------------------------
	string ParseBody_Normal(const string& ln)
	{
		// 다른 스테이트로 트랜지션
		if (ln == "<TAG>")
		{
			parseBodyFunc = &Parser::ParseBody_Tag;
			return "\n";
		}
		if (starts_with(ln, "<code:"))
		{
			string rest = ln.substr(6);
			size_t e = rest.find('>');

			parseBodyFunc = &Parser::ParseBody_Code;
			return "<pre name='code' class='" + rest.substr(0, e) + "'>\n";
		}
		if (ln == "{{{")
		{
			parseBodyFunc = &Parser::ParseBody_Quote;
			return "<div class=quote>\n";
		}

		// 특수 라인
		if (ln.empty())
		{
			return "<p class='emptyLine'></p>\n"; // 빈줄 높이 제어
		}
		if (starts_with(ln, "-----"))
		{
			return "<hr/>";
		}
		if (starts_with(ln, "## "))
		{
			return "<h2>" + ln.substr(3) + "</h2>\n";
		}
		if (starts_with(ln, "### "))
		{
			return "<h3>" + ln.substr(4) + "</h3>\n";
		}
		if (starts_with(ln, "#### "))
		{
			return "<h4>" + ln.substr(5) + "</h4>\n";
		}

		// 내용물 변환
		string line = ln;
		line = ConvertLinks(line);

		// 마무리
		if (starts_with(line, " * "))
		{
			return "<li>" + line.substr(3) + "</li>\n";
		}
		else
		{
			string klass;
			for (int i = 1; i <= 5; ++i)
			{
				if (starts_with(line, string(i, ' ')))
				{
					klass = "indent" + ToString(i);
				}
			}

			if (klass.empty())
			{
				return "<p>" + line + "</p>\n";
			}
			else
			{
				return "<p class=" + klass + ">" + line + "</p>\n";
			}
		}
	}

	//-------------------------------------------------------------------------
	string ParseBody_Tag(const string& ln)
	{
		if (ln == "</TAG>")
		{
			parseBodyFunc = &Parser::ParseBody_Normal;
			return "";
		}
		else
		{
			return ln; // pass-through
		}
	}

	//-------------------------------------------------------------------------
	string ParseBody_Code(const string& ln)
	{
		if (ln == "</code>")
		{
			parseBodyFunc = &Parser::ParseBody_Normal;
			return "</pre>\n";
		}
		else
		{
			return EscapeTag(ln) + "\n";
		}
	}

	//-------------------------------------------------------------------------
	string EscapeTag(string s)
	{
		s = StrReplace<string>(s, "<", "&lt;");
		s = StrReplace<string>(s, ">", "&gt;");
		return s;
	}

	//-------------------------------------------------------------------------
	string ParseBody_Quote(const string& ln)
	{
		if (ln == "}}}")
		{
			parseBodyFunc = &Parser::ParseBody_Normal;
			return "</div>\n";
		}
		else
		{
			return EscapeTag(ln) + "<br/>\n";
		}
	}

	//-------------------------------------------------------------------------
	string LinkURL(string ln, string prefix, string exceptAfter)
	{
		string rv;

		size_t b = ln.find(prefix);
		while (b != string::npos)
		{
			if (b > 0 && prefix.find(ln[b - 1]) != string::npos)
			{
				b = ln.find(prefix, b + 1);
			}
			else
			{
				size_t e = b;
				while (e < ln.size() && ln[e] != ' ') { ++e; }

				string link = ln.substr(b, e - b);

				rv += ln.substr(0, b);
				rv += "<a href='";
				rv += link;
				rv += "' class=externalLink>";
				rv += link;
				rv += "</a>";

				ln = ln.substr(e);
				b = ln.find(prefix, e);
			}
		}

		return rv + ln;
	}

	//-------------------------------------------------------------------------
	string ConvertLinks(string ln)
	{
		// 1. http://... 를 직접 변환
		{
			ln = LinkURL(ln, "http://", ":[");
			ln = LinkURL(ln, "https://", ":[");
		}

		// 2. [캡션:링크], [링크] 패턴을 추가로 변환
		size_t b = string::npos;
		for (;;)
		{
			size_t e = ln.find_last_of(']', b);
			if (e == string::npos)
			{
				break;
			}
			b = ln.find_last_of('[', e);
			if (b == string::npos)
			{
				break;
			}

			string caption;
			string link;
			bool isExternal = false;

			size_t sep = ln.find(':', b);
			if (sep == string::npos || sep > e)
			{
				link = ln.substr(b + 1, e - b - 1);
				if (starts_with(link, "http://") || starts_with(link, "https://"))
				{
					caption = link;
					isExternal = true;
				}
				else
				{
					caption = linkProvider.GetTitle(ucs2_to_utf8(docID), link);
				}
				link += extHtml8;
			}
			else
			{
				link = ln.substr(sep + 1, e - sep - 1);
				caption = ln.substr(b + 1, sep - b - 1);

				if (starts_with(link, "http://") || starts_with(link, "https://"))
				{
					isExternal = true;
				}
				else
				{
					linkProvider.GetTitle(ucs2_to_utf8(docID), link);
				}
			}
			string klass = isExternal ? "externalLink" : "internalLink";
			string tobe = "<A href='" + link + "' class=" + klass + ">" + caption + "</A>";
			ln = ln.substr(0, b) + tobe + ln.substr(e + 1);
		}
		//wcout << L"{" + utf8_to_ucs2(ln) + L"}" << endl;

		return ln;
	}

	//-------------------------------------------------------------------------
	void SkipEmptyLine()
	{
		string ln;
		getline(fin, ln);
		if (!ln.empty())
		{
			Error(L"빈 줄이 와야 하는데 그렇지 않습니다", ln);
		}
	}

	//-------------------------------------------------------------------------
	void Error(wstring what, string ln)
	{
		wcout << docID << L" - " << what << L" : [" << utf8_to_ucs2(ln) << L"]" << endl;
		exit(1);
	}

	//-------------------------------------------------------------------------
	void ExportTo(TextContent& tc)
	{
		tc.AddString("$title", title);
		tc.AddString("$firstDate", firstDate);
		tc.AddString("$lastDate", lastDate);
		tc.AddString("$body", body);
	}

protected:
	//-------------------------------------------------------------------------
	wstring docID;
	fstream fin;

	string title;
	string firstDate;
	string lastDate;
	string tags;
	string body;
	vector<string> links;

	const LinkProvider& linkProvider;
	DocumentList& docList;
};

///////////////////////////////////////////////////////////////////////////////
class StdoutOutputStream : public OutputStream
{
public:
	virtual void Output( const char* text, int size )
	{
		for (int i = 0; i < size; ++i)
		{
			printf("%c", text[i]);
		}
	}
};

///////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");

	//----------------------------------
	// 컨피그 읽기
	{
		ifstream config("config");
		if (!config.is_open())
		{
			printf("unable to open config\n");
			return -1;
		}

		string skinFile8;	::getline(config, skinFile8);
		string htmlDir8;	::getline(config, htmlDir8);
		string textDir8;	::getline(config, textDir8);
		skinFile = utf8_to_ucs2(skinFile8);
		htmlDir = utf8_to_ucs2(htmlDir8);
		textDir = utf8_to_ucs2(textDir8);
	}

	//----------------------------------
	// 스킨 읽기
	TextTemplate skin;
	{
		FILE* fp = NULL;
		fopen_s(&fp, "skin.html", "rt");
		if (fp == NULL)
		{
			printf("unable to open skin\n");
			return -1;
		}

		fseek(fp, 0, SEEK_END);
		long skinSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		vector<char> skinBuffer;
		skinBuffer.resize(skinSize);
		fread(&skinBuffer[0], skinSize, 1, fp);

		if (!skin.Load(&skinBuffer[0], skinSize))
		{
			printf("unable to load skin\n");
			return -1;
		}
		fclose(fp);
	}

	//----------------------------------
	// gather file names
	for( file_iterator<wstring> it(textDir + L"*" + textExt), end; it!=end; ++it )
	{
		if( !it.is_directory() )
		{
			wstring filename = *it;
			filename = filename.substr(0, filename.length() - textExt.length());
			docNames.insert(filename);
		}
	}

	DocumentList docList;
	DummyLinkProvider dummy;

	//----------------------------------
	// gather information
	for (set<wstring>::iterator it = docNames.begin(); it != docNames.end(); ++it)
	{
		Parser parser(*it, dummy, docList);

		parser.ParseHeader();
		parser.ParseBody();
	}

	//----------------------------------
	// convert docs
	for (set<wstring>::iterator it = docNames.begin(); it != docNames.end(); ++it)
	{
		Parser parser(*it, docList, docList);

		parser.ParseHeader();
		parser.ParseBody();

		TextContent tc;
		parser.ExportTo(tc);

		wstring wpath = (htmlDir + *it + htmlExt);
		fstream fout(wpath.c_str(), fstream::out);
		if (!fout.is_open())
		{
			wcout << L"파일을 쓰지 못함: " << wpath << endl;
		}
		FileOutputStream sos(fout);
		skin.Print(tc, sos);
	}

	//----------------------------------
	// write index
	{
		TextContent indexTC;
		docList.ExportTo(indexTC);
		wstring wpath = (htmlDir + L"index.html");
		fstream fout(wpath.c_str(), fstream::out);
		if (!fout.is_open())
		{
			wcout << L"파일을 쓰지 못함: " << wpath << endl;
		}
		FileOutputStream sos(fout);
		skin.Print(indexTC, sos);
	}

	printf("Converted %d posts.\n", docNames.size());

	return 0;
}

