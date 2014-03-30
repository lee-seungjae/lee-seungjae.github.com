#ifndef _UPNL_TYPES_H_INCLUDED_
#define _UPNL_TYPES_H_INCLUDED_

namespace UPnL
{	
	typedef unsigned char		byte;
	typedef signed char			int8;
	typedef unsigned char		uint8;
	typedef short				int16;
	typedef unsigned short		uint16;
	typedef int					int32;
	typedef unsigned int		uint32;
	typedef unsigned int		uint;

#ifdef WIN32
	typedef __int64				int64;
	typedef unsigned __int64	uint64;
#else
	typedef long long			int64;
	typedef unsigned long long	uint64;
#endif

	class Noncopyable
	{
	protected:
		Noncopyable() { }
		~Noncopyable() { }
	private:
		Noncopyable( const Noncopyable& );
		const Noncopyable& operator = ( const Noncopyable& );
	};

}; // namespace UPnL

#endif
