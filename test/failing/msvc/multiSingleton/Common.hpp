#include <rod/Resolve.hpp>
#include <rod/annotation/Requires.hpp>




struct SomeComponent
{};


struct MultiSingletonDependency
{};


struct MultiSingleton
{
	using Requires = rod::annotation::Requires< MultiSingletonDependency& >;

	MultiSingleton( MultiSingletonDependency& )
	{}
};


struct RequiringMultiSingleton
{
	using Requires = rod::annotation::Requires< MultiSingleton& >;

	RequiringMultiSingleton( MultiSingleton& )
	{}
};


template< typename Context >
struct MultiSingletonResolver
{
	MultiSingleton&	multiSingleton;

	MultiSingletonResolver( Context& context ):
	  multiSingleton( rod::resolve< MultiSingleton& >( context ) )
	{}
};