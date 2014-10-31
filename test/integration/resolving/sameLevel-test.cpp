#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>




bool called = false;


struct Component
{
	void
	method()
	{
		called = true;
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto withComponent = rod::extend( root ).with< rod::Singleton< Component > >()();
		rod::resolve< Component& >( withComponent ).method();
	});

	assert( called );
}