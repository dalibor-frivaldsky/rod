#include <cassert>
#include <type_traits>

#include <rod/Context.hpp>
#include <rod/Injected.hpp>




int destructorsCalled = 0;
int constructorsCalled = 0;
int moveConstructorsCalled = 0;
int referenceConstructorsCalled = 0;

struct Component
{
	Component()
	{
		++constructorsCalled;
	}

	Component( Component& )
	{
		++referenceConstructorsCalled;
	}

	Component( Component&& )
	{
		++moveConstructorsCalled;
	}

	~Component()
	{
		++destructorsCalled;
	}
};


void
test()
{
	{
		using context = rod::CreateInitialContext<
								rod::Injected< Component& >
						>::r;

		auto		nullCtx = rod::createNullContext();
		Component	c;
		context	ctx( nullCtx, c );
	}

	assert( constructorsCalled == 1 );
	assert( referenceConstructorsCalled == 0 );
	assert( moveConstructorsCalled == 0 );
	assert( destructorsCalled == 1 );
}