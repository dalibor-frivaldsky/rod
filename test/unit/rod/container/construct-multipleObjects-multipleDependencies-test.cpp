#include <cassert>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "../../MockContext.hpp"




struct C1
{
	using ConstructWith = rod::annotation::ConstructWith< int, float >;

	C1( int a, float b )
	{
		assert( a == 10 );
		assert( b == 1.0f );
	}
};

struct C2
{
	using ConstructWith = rod::annotation::ConstructWith< float, double >;

	C2( float a, double b )
	{
		assert( a == 1.0f );
		assert( b == 2.0 );
	}
};


void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< C1 >,
					rod::Singleton< C2 >
			  >::r;


	auto	mock = mockContext( 2.0, 10, 1.0f );
	c	cont( mock, std::make_tuple() );
}