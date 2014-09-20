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
	static_assert( std::is_same<
						rod::CreateInitialContext<
							Type,
							rod::Singleton< Component >
						>::r,
						rod::Context<
							rod::CreateContextLevel<
								Type,
								rod::Singleton< Component >
							>::r
						>
				   >::value,
				   "Unexpected Context structure" );
}