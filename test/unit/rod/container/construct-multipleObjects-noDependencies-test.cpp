#include <cassert>

#include <rod/AsSingleton.hpp>
#include <rod/Container.hpp>




int called = 0;


struct C1
{
	using ConstructWith = rod::annotation::ConstructWith<>;

	C1()
	{
		++called;
	}
};

struct C2
{
	using ConstructWith = rod::annotation::ConstructWith<>;

	C2()
	{
		++called;
	}
};


void
test()
{
	using c = rod::CreateContainer<
					rod::AsSingleton< C1 >,
					rod::AsSingleton< C2 >
			  >::r;

	c	cont( std::make_tuple() );
	assert( called == 2 );
}