#include "rod/Context.hpp"


#include <type_traits>

#include "rod/AsSingleton.hpp"




struct Type
{};

struct Component
{};


void
test()
{
	using context = rod::CreateInitialContext<
							Type,
							rod::AsSingleton< Component >
					>::r;

	auto	nullCtx = rod::createNullContext();
	context	ctx( nullCtx );
}