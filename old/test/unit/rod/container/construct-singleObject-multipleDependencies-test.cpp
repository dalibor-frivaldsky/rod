#include <cassert>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "../../MockContext.hpp"




bool called = false;


struct C
{
	using Requires = rod::annotation::Requires< int, float >;

	C( int, float )
	{
		called = true;
	}
};


void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< C >
			  >::r;

	auto	mock = mockContext( 10, 1.0f );
	c	cont( mock, std::make_tuple() );
	assert( called );
}