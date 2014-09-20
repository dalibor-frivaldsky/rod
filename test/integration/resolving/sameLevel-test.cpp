#include "rod/Rod.hpp"


#include <cassert>

#include "rod/Singleton.hpp"
#include "rod/Contextual.hpp"




bool called = false;


struct Component
{
	void
	method()
	{
		called = true;
	}
};


template< typename Context >
class Domain:
  public rod::Contextual< Context, rod::Singleton< Component > >
{
private:

	Component&	component = ROD_Resolve( Component& );

public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		component.method();
	}
};


void
test()
{
	rod::enterPlain< Domain >();

	assert( called );
}