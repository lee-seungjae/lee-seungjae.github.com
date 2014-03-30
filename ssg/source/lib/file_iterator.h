#ifndef UPNL_FILE_ITERATOR_H_INCLUDED_
#define UPNL_FILE_ITERATOR_H_INCLUDED_

#include <iterator>
#include <string>
#include <cassert>

namespace UPnL
{
	template< typename STR_TYPE=std::string >
	class file_iterator	: public std::iterator<std::input_iterator_tag, STR_TYPE>
	{
	public:
		// 검색의 시작을 지시하는 생성자
		file_iterator( const STR_TYPE filter ) : isEndIterator(false)
		{
            
			HANDLE handle = ::FindFirstFile(filter.c_str(), &data);
			if( handle == INVALID_HANDLE_VALUE )
			{
				pHandle = NULL;
			}
			else
			{
				pHandle = new InternalHandle(handle);
			}            
		}

		// 검색의 종료를 지시하는 생성자
		file_iterator() : pHandle(NULL), isEndIterator(true)
		{
		}

		// 복사 생성자, 대입 연산자
		file_iterator( const file_iterator& rhs ) : pHandle(NULL)
		{
			(*this) = rhs;
		}
		const file_iterator& operator = ( const file_iterator& rhs )
		{
			// 아직까지 들고있는 핸들이 있었다면 파기한다
			if( pHandle )
			{
				pHandle->Destroy();
			}

			// 대입할 대상이 핸들을 들고 있으면 복사해 오고 아니면 null로 채운다
			if( rhs.pHandle )
			{
				pHandle = rhs.pHandle->Clone();
			}
			else
			{
				pHandle = NULL;
			}

			// 잡다한 멤버 복사
			isEndIterator = rhs.isEndIterator;
			data          = rhs.data;

			return *this;
		}

		// 소멸자
		~file_iterator()
		{
			if( pHandle )
			{
				pHandle->Destroy();
			}
		}

		// accessors
		const STR_TYPE operator*() const
		{
			return data.cFileName;
		}
		bool is_valid() const
		{
			return isEndIterator || pHandle!=NULL;
		}
		bool is_end() const
		{
			return isEndIterator || pHandle==NULL || pHandle->IsEnd();
		}
		bool is_directory() const
		{
			return !!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		}
		bool is_hidden() const
		{
			return !!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
		}
		bool is_archive() const
		{
			return !!(data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
		}
		bool is_readonly() const
		{
			return !!(data.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
		}
		bool is_system() const
		{
			return !!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
		}

		FILETIME date() const
		{
			return data.ftLastWriteTime;
		}

		ULONGLONG size() const
		{
			ULARGE_INTEGER res;
			res.LowPart  = data.nFileSizeLow;
			res.HighPart = data.nFileSizeHigh;

			return res.QuadPart;
		}
		bool operator==( const file_iterator& rhs ) const
		{
			// 비교할 때 두 이터레이터 중 적어도 하나는 반드시 end여야만 한다.
			assert(is_end() || rhs.is_end());
			return is_end() && rhs.is_end();
		}
		bool operator!=( const file_iterator& rhs ) const
		{
			return !(*this == rhs);
		}

		const file_iterator& operator++()
		{
			assert(pHandle != NULL);
			pHandle->Advance(&data);
			return *this;
		}
		const file_iterator& operator++( int )
		{
			assert(pHandle != NULL);
			pHandle->Advance(&data);
			return *this;
		}

	private:
		class InternalHandle
		{
		public:
			InternalHandle( HANDLE h )
			{
				assert(h != INVALID_HANDLE_VALUE);
				handle   = h;
				refCount = 1;
				isEnd    = false;
			}
			InternalHandle* Clone()
			{
				refCount++;
				return this; 
			}
			void Destroy()
			{
				refCount--;
				if( refCount==0 )
					delete this;
			}
			void Advance( WIN32_FIND_DATA* pData )
			{
				assert(isEnd == false);
				if( ::FindNextFile(handle, pData) == 0 )
				{
					isEnd = true;
				}
			}
			bool IsEnd()
			{
				return isEnd;
			}
		private:
			HANDLE  handle;
			bool    isEnd;
			int     refCount;

			InternalHandle( const InternalHandle& rhs );
			void operator = ( const InternalHandle& rhs );
			~InternalHandle()
			{
				assert(refCount==0);
				FindClose(handle);
			}
		};

        bool             isEndIterator;
		InternalHandle   *pHandle;
		WIN32_FIND_DATA  data;
	};
}

#endif