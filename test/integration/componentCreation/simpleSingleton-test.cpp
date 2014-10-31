#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>




bool called = false;


struct Component
{
	Component()
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
		auto	withComponent = rod::extend( root ).with< rod::Singleton< Component > >()();
	});

	assert( called );
}