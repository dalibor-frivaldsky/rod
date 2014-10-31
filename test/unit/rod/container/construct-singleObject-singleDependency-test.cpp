#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "../../MockContext.hpp"




struct A
{};

struct C
{
	using Requires = rod::annotation::Requires< A& >;

	C( A& )
	{}
};


void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< C >
			  >::r;

	A	a;
	auto	mock = mockContext( a );
	c	cont( mock, std::make_tuple() );
}