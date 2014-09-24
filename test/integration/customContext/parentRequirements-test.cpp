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
		auto contextWithComponent = rod::extend( root ).with< rod::Singleton< Component > >()();
		auto testContext = rod::extend( contextWithComponent ).as< TestContext >()();
	});

	assert( called );
}