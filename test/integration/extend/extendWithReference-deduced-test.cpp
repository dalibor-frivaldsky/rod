#include <rod/Extend.hpp>
#include <rod/Resolve.hpp>
#include <rod/Rod.hpp>




struct Object
{};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		Object	object;
		auto	context = rod::extend( root ).with( object )();

		rod::resolve< Object& >( context );
	});
}