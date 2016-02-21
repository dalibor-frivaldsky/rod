#include <cassert>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "../../MockContext.hpp"




int called = 0;


struct C1
{
	using Requires = rod::annotation::Requires< int >;

	C1( int )
	{
		++called;
	}
};

struct C2
{
	using Requires = rod::annotation::Requires< float >;

	C2( float )
	{
		++called;
	}
};


void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< C1 >,
					rod::Singleton< C2 >
			  >::r;

	auto	mock = mockContext( 10, 1.0f );
	c	cont( mock, std::make_tuple() );
	assert( called == 2 );
}