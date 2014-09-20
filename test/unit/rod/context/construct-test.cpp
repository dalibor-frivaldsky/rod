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
							Type,
							rod::Singleton< Component >
					>::r;

	auto	nullCtx = rod::createNullContext();
	context	ctx( nullCtx );
}