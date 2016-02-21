#include "rod/Context.hpp"


#include <type_traits>

#include "rod/Singleton.hpp"




struct Type
{};

struct Component
{};

struct Component2
{};


void
test()
{
	using initial = rod::CreateInitialContext<
							Type
						>::r;
	using child = initial::CreateChildContext<
							rod::Singleton< Component >
						>::r;

	static_assert( std::is_same<
						child::Enrich< rod::Singleton< Component2 > >::r,
						rod::Context<
							rod::CreateContextLevel<
								rod::Singleton< Component >,
								rod::Singleton< Component2 >
							>::r,
							rod::CreateContextLevel<
								Type
							>::r
						>
				   >::value,
				   "Unexpected Enriched Context structure" );
}