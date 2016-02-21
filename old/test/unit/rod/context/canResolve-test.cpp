#include "rod/Context.hpp"


#include "rod/Singleton.hpp"




bool called;

struct Component
{};


struct NotComponent
{};


void
test()
{
	using context = rod::CreateInitialContext<
							rod::Singleton< Component >
					>::r;

	static_assert( context::CanResolve< Component& >::r, "Expected to resolve" );
	static_assert( context::CanResolve< NotComponent& >::r == false, "Expected not to resolve" );
}