#include "rod/Context.hpp"


#include <cassert>

#include "rod/Singleton.hpp"




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
							rod::Singleton< Component >
					>::r;

	auto	nullCtx = rod::createNullContext();
	context	ctx( nullCtx );

	ctx.resolve< Component& >().method();
}