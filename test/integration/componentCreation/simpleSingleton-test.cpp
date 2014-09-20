#include "rod/Rod.hpp"


#include <cassert>

#include "rod/Singleton.hpp"
#include "rod/Contextual.hpp"




bool called = false;


struct Component
{
	Component()
	{
		called = true;
	}
};


template< typename Context >
class Domain:
  public rod::Contextual< Context, rod::Singleton< Component > >
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

	assert( called );
}