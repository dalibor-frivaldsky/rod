#include <cassert>

#include <rod/AsSingleton.hpp>
#include <rod/Container.hpp>




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
					rod::AsSingleton< C1 >,
					rod::AsSingleton< C2 >
			  >::r;

	c	cont( std::forward_as_tuple( [] { return 2.0; }, [] { return 10; }, [] { return 1.0f; } ) );
}