#include <rod/Extend.hpp>
#include <rod/Injected.hpp>
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
		rod::Extend< rod::Root >
			::With< rod::Injected< Object > >::r		context( root, Object() );

		rod::resolve< Object& >( context );
	});
}