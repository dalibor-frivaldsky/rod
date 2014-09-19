#include "rod/Context.hpp"


#include <cassert>
#include <iostream>
#include <type_traits>

#include "rod/Injected.hpp"




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
								rod::Injected< Component&& >
						>::r;

		auto		nullCtx = rod::createNullContext();
		context	ctx( nullCtx, Component() );
	}

	assert( constructorsCalled == 1 );
	assert( referenceConstructorsCalled == 0 );
	assert( moveConstructorsCalled == 1 );
	assert( destructorsCalled == 2 );
}