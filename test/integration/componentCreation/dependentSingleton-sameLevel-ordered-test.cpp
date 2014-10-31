#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>
#include <rod/annotation/Requires.hpp>




int called = 0;


struct Component
{
	Component()
	{
		assert( called == 0 );
		++called;
	}

	void
	method()
	{
		++called;
	}
};

struct DependentComponent
{
	using Requires = rod::annotation::Requires< Component& >;

	DependentComponent( Component& component )
	{
		assert( called == 1 );
		++called;

		component.method();
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto	extended = rod::extend( root )
								.with<
									rod::Singleton< Component >,
									rod::Singleton< DependentComponent > >()();
	});

	assert( called == 3 );
}