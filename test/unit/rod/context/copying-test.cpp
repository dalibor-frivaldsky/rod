#include "rod/Context.hpp"


#include <type_traits>

#include "rod/Singleton.hpp"




struct Type
{};

struct Component
{};


void
test()
{
	using context = rod::CreateInitialContext<
							Type
					>::r;

	auto		nullCtx = rod::createNullContext();
	context		ctx( nullCtx, Component() );
	context 	ctx2( ctx );
}