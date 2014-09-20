#include "rod/Rod.hpp"


#include <cassert>

#include "rod/Singleton.hpp"
#include "rod/Contextual.hpp"




bool called = false;


class Interface
{
public:
	virtual void	method() = 0;
};


class Component:
	public Interface
{
public:
	virtual
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

	Interface&	interface = ROD_Resolve( Interface& );

public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		interface.method();
	}
};


void
test()
{
	rod::enterPlain< Domain >();

	assert( called );
}