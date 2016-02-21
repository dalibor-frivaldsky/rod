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


struct DummyComponent
{};


void
test()
{
	using context = rod::CreateInitialContext<
							rod::Singleton< Component >
					>::r;
	using broaderContext = context::CreateChildContext< rod::Singleton< DummyComponent > >::r;

	auto	nullCtx = rod::createNullContext();
	context	ctx( nullCtx );
	broaderContext	broaderCtx( ctx );

	broaderCtx.resolve< Component& >().method();
}