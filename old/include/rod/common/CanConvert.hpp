#pragma once




namespace rod
{

	namespace common
	{

		template< typename From, typename To >
		struct CanConvert
		{
		private:
			typedef char	Small;
			struct Big { char a[ 2 ]; };
			static Small	test( const To& );
			static Big		test( ... );
			static From	makeFrom();
		public:
			enum { r = sizeof( test( makeFrom() ) ) == sizeof( Small ) };
		};
		
	}

}