#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
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


template< typename Context >
class TestContext
{
public:
	using Requires = rod::annotation::Requires< Component& >;

	TestContext( Context&, Component& component )
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
		Component	component;
		auto testContext = rod::extend( root )
								.with( component )
								.as< TestContext >()();
	});

	assert( called );
}