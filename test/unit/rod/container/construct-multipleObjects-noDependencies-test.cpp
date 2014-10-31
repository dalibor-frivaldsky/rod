#include <cassert>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "../../MockContext.hpp"




int called = 0;


struct C1
{
	using Requires = rod::annotation::Requires<>;

	C1()
	{
		++called;
	}
};

struct C2
{
	using Requires = rod::annotation::Requires<>;

	C2()
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

	auto	mock = mockContext();
	c	cont( mock, std::make_tuple() );
	assert( called == 2 );
}