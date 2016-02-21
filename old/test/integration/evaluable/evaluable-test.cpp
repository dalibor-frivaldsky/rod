#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Evaluable.hpp>
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


struct CallMethod
{
	template< typename Context >
	void
	operator () ( Context& context )
	{
		rod::resolve< Component& >( context ).method();
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
									rod::Singleton< Component > >()();

		auto evaluate = rod::Evaluable< CallMethod >( extended );
		evaluate( CallMethod() );
	});

	assert( called );
}