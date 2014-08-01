#include "rod/Rod.hpp"


#include <cassert>

#include "rod/Contextual.hpp"




bool called = false;


struct Injected
{
	void
	method()
	{
		called = true;
	}
};


template< typename Context >
class Domain:
  public rod::Contextual< Context >
{
public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		rod::resolve< Injected& >( this ).method();
	}
};


void
test()
{
	Injected	i;
	rod::enter< Domain >( i );

	assert( called );
}