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


struct DummyComponent
{};


void
test()
{
	using context = rod::CreateInitialContext<
							rod::AsSingleton< Component >
					>::r;
	using broaderContext = context::CreateChildContext< rod::AsSingleton< DummyComponent > >::r;

	auto	nullCtx = rod::createNullContext();
	context	ctx( nullCtx );
	broaderContext	broaderCtx( ctx );

	broaderCtx.resolve< Component& >().method();
}