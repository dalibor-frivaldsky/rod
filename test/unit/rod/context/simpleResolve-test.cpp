#include "rod/Context.hpp"


#include <cassert>

#include "rod/AsSingleton.hpp"




bool called;

struct Component
{
	void
	method()
	{
		called = true;
	}
};


void
test()
{
	using context = rod::CreateInitialContext<
							rod::AsSingleton< Component >
					>::r;

	auto	nullCtx = rod::createNullContext();
	context	ctx( nullCtx );

	ctx.resolve< Component& >().method();
}