#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>
#include <rod/With.hpp>




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

		rod::with( withComponent,
		[] ( Component& component )
		{
			component.method();
		});
	});

	assert( called );
}