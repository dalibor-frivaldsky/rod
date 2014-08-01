#include "rod/Contextual.hpp"


#include <cassert>
#include <memory>

#include "rod/Memory.hpp"

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
		auto test = rod::asUnique( rod::createPtr< Test >( this ) );
		test->method();
	}
};


void
test()
{
	enter< TestEntry >();
	assert( called );
}