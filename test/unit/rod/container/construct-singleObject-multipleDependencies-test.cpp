#include <cassert>

#include <rod/AsSingleton.hpp>
#include <rod/Container.hpp>




bool called = false;


struct C
{
	using ConstructWith = rod::annotation::ConstructWith< int, float >;

	C( int, float )
	{
		called = true;
	}
};


void
test()
{
	using c = rod::CreateContainer<
					rod::AsSingleton< C >
			  >::r;

	c	cont( std::make_tuple( [] { return 10; }, [] { return 1.0f; } ) );
	assert( called );
}