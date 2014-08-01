#include "rod/Context.hpp"


#include <type_traits>

#include "rod/AsSingleton.hpp"




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
							rod::AsSingleton< Component >
						>::r;

	static_assert( std::is_same<
						child::Enrich< rod::AsSingleton< Component2 > >::r,
						rod::Context<
							rod::CreateContextLevel<
								rod::AsSingleton< Component >,
								rod::AsSingleton< Component2 >
							>::r,
							rod::CreateContextLevel<
								Type
							>::r
						>
				   >::value,
				   "Unexpected Enriched Context structure" );
}