#include "rod/Rod.hpp"


#include <cassert>

#include "rod/Contextual.hpp"




bool called = false;


template< typename Context >
class Domain:
  public rod::Contextual< Context >
{
public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		called = true;
	}
};


void
test()
{
	rod::enter< Domain >();

	assert( called );
}