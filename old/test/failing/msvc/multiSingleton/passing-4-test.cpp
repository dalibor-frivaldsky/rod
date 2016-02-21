#include "Common.hpp"

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>




void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto	extended = rod::extend( root )
								.with<
									rod::Singleton< MultiSingletonDependency >,
									rod::Singleton< MultiSingleton > >()();

		MultiSingletonResolver< decltype( extended ) >	resolver1( extended );

		auto	requiring = rod::extend( extended )
								.with<
									rod::Singleton< RequiringMultiSingleton > >()();
	});
}