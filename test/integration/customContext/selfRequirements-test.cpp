#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>
#include <rod/annotation/ExtendWith.hpp>
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
	using ExtendWith = rod::annotation::ExtendWith< rod::Singleton< Component > >;

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
		auto testContext = rod::extend( root ).as< TestContext >()();
	});

	assert( called );
}