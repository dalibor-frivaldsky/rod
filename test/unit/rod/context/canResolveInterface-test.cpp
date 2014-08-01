#include "rod/Context.hpp"


#include "rod/AsSingleton.hpp"




bool called;

struct Interface
{};


struct Component:
	public Interface
{};


void
test()
{
	using context = rod::CreateInitialContext<
							rod::AsSingleton< Component >
					>::r;

	static_assert( context::CanResolve< Interface& >::r, "Expected to resolve by interface" );
}