#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Resolve.hpp>
#include <rod/Rod.hpp>
#include <rod/Within.hpp>




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
		rod::within(
			root, Component(),
		CallMethod() );
	});

	assert( called );
}