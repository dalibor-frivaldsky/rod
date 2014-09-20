#include "rod/Context.hpp"


#include "rod/Singleton.hpp"




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
							rod::Singleton< Component >
					>::r;

	static_assert( context::CanResolve< Interface& >::r, "Expected to resolve by interface" );
}