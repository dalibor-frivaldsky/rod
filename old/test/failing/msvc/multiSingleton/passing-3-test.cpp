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
		auto	withSomeComponent = rod::extend( root )
										.with<
											rod::Singleton< SomeComponent > >()();
		auto	extended1 = rod::extend( withSomeComponent )
								.with<
									rod::Singleton< MultiSingletonDependency >,
									rod::Singleton< MultiSingleton > >()();
		MultiSingletonResolver< decltype( extended1 ) >	resolver1( extended1 );


		auto	extended2 = rod::extend( root )
								.with<
									rod::Singleton< MultiSingletonDependency >,
									rod::Singleton< MultiSingleton > >()();
		/*auto	requiring2 = rod::extend( extended2 )
								.with<
									rod::Singleton< RequiringMultiSingleton > >()();*/
	});
}