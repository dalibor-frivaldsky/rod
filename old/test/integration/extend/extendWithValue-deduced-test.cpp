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
		auto	context = rod::extend( root ).with( Object() )();

		rod::resolve< Object& >( context );
	});
}