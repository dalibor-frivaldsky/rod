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
		auto	context = rod::extend( root ).with< rod::Singleton< Component > >()();

		rod::resolve< Component& >( context );
	});
}