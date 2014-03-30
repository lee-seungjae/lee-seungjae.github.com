#pragma once

#include "OutputStream.h"
#include <fstream>

namespace UPnL
{
	
	class FileOutputStream : public OutputStream
	{
	public:
		FileOutputStream(fstream& f) : f(f)
		{
		}
		
		virtual void Output( const char* text, int size )
		{
			f.write(text, size);
		}

		fstream& f;
	};
}

