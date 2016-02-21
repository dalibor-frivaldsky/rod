#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>
#include <rod/annotation/Requires.hpp>




bool called = false;


struct Component
{
	void
	method()
	{
		called = true;
	}
};

struct DependentComponent
{
	using Requires = rod::annotation::Requires< Component& >;

	DependentComponent( Component& component )
	{
		component.method();
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto	withComponent = rod::extend( root ).with< rod::Singleton< Component > >()();
		rod::extend( withComponent ).with< rod::Singleton< DependentComponent > >()();
	});

	assert( called );
}