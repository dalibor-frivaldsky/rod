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
		Object	object;

		rod::Extend< rod::Root >
			::With< rod::Injected< Object& > >::r		context( root, object );

		rod::resolve< Object& >( context );
	});
}