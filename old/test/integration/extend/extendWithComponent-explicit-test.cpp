#include <type_traits>

#include <rod/Extend.hpp>
#include <rod/Resolve.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>




struct Component
{};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		rod::Extend< rod::Root >
			::With< rod::Singleton< Component > >::r		context( root );

		rod::resolve< Component& >( context );
	});
}