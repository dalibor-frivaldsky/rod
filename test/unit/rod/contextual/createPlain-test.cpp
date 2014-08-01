#include "rod/Contextual.hpp"


#include <cassert>

#include "Common.hpp"




bool called = false;


template< typename Context >
class Test:
	public rod::Contextual< Context >
{
public:

	ROD_Contextual_Constructor( Test )

	void
	method()
	{
		called = true;
	}
};


template< typename Context >
class TestEntry:
	public rod::Contextual< Context >
{
public:

	ROD_Contextual_Constructor( TestEntry )

	void
	enter()
	{
		rod::create< Test >( this ).method();
	}
};


void
test()
{
	enter< TestEntry >();
	assert( called );
}