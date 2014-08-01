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
	static_assert( std::is_same<
						rod::CreateInitialContext<
							Type,
							rod::AsSingleton< Component >
						>::r,
						rod::Context<
							rod::CreateContextLevel<
								Type,
								rod::AsSingleton< Component >
							>::r
						>
				   >::value,
				   "Unexpected Context structure" );
}