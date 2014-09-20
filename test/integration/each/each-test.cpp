#include <cassert>

#include "rod/Singleton.hpp"
#include "rod/Contextual.hpp"
#include "rod/Each.hpp"
#include "rod/Rod.hpp"




int value = 0;


struct Interface
{
	virtual void	method() = 0;
};

struct Implementor1:
	public Interface
{
	virtual
	void
	method()
	{
		value += 1;
	}
};

struct Implementor2:
	public Interface
{
	virtual
	void
	method()
	{
		value += 2;
	}
};


template< typename Context >
class Domain:
  public rod::Contextual<
  				Context,
  				rod::Singleton< Implementor1 >,
  				rod::Singleton< Implementor2 > >
{
public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		rod::each< Interface >( this, [] ( Interface& i )
		{
			i.method();
		});
	}
};


void
test()
{
	rod::enterPlain< Domain >();

	assert( value == 3 );
}