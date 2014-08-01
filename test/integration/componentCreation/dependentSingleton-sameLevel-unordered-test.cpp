#include "rod/Rod.hpp"


#include <cassert>

#include "rod/AsSingleton.hpp"
#include "rod/Contextual.hpp"
#include "rod/annotation/ConstructWith.hpp"




int called = 0;


struct Component
{
	Component()
	{
		assert( called == 0 );
		++called;
	}

	void
	method()
	{
		++called;
	}
};

struct DependentComponent
{
	using ConstructWith = rod::annotation::ConstructWith< Component& >;

	DependentComponent( Component& component )
	{
		assert( called == 1 );
		++called;

		component.method();
	}
};


template< typename Context >
class Domain:
  public rod::Contextual< Context,
  						  rod::AsSingleton< DependentComponent >,
  						  rod::AsSingleton< Component > >
{
public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{}
};


void
test()
{
	rod::enterPlain< Domain >();

	assert( called == 3 );
}