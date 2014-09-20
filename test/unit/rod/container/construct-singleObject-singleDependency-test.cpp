#include <rod/Singleton.hpp>
#include <rod/Container.hpp>




struct A
{};

struct C
{
	using ConstructWith = rod::annotation::ConstructWith< A& >;

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
	c	cont( std::make_tuple( [&a] () -> A& { return a; } ) );
}