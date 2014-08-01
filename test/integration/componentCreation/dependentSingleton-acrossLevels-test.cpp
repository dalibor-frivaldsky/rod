#include "rod/Rod.hpp"


#include <cassert>

#include "rod/AsSingleton.hpp"
#include "rod/Contextual.hpp"
#include "rod/annotation/ConstructWith.hpp"




bool called = false;


struct Component
{
	void
	method()
	{
		called = true;
	}
};

struct DependentComponent
{
	using ConstructWith = rod::annotation::ConstructWith< Component& >;

	DependentComponent( Component& component )
	{
		component.method();
	}
};


template< typename Context >
class BroaderDomain:
  public rod::Contextual< Context, rod::AsSingleton< DependentComponent > >
{
public:

	ROD_Contextual_Constructor( BroaderDomain )

	void
	enter()
	{}
};


template< typename Context >
class Domain:
  public rod::Contextual< Context, rod::AsSingleton< Component > >
{
public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		rod::create< BroaderDomain >( this ).enter();
	}
};


void
test()
{
	rod::enterPlain< Domain >();

	assert( called );
}