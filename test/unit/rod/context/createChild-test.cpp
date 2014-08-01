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
	using initial = rod::CreateInitialContext<
							Type
						>::r;
	using child = initial::CreateChildContext<
							rod::AsSingleton< Component >
						>::r;

	static_assert( std::is_same<
						child,
						rod::Context<
							rod::CreateContextLevel<
								rod::AsSingleton< Component >
							>::r,
							rod::CreateContextLevel<
								Type
							>::r
						>
				   >::value,
				   "Unexpected Child Context structure" );
}