#include <cassert>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>




int called = 0;


struct C1
{
	using ConstructWith = rod::annotation::ConstructWith< int >;

	C1( int )
	{
		++called;
	}
};

struct C2
{
	using ConstructWith = rod::annotation::ConstructWith< float >;

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

	c	cont( std::make_tuple( [] { return 10; }, [] { return 1.0f; } ) );
	assert( called == 2 );
}