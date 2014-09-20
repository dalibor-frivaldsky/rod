#include "rod/Context.hpp"


#include <type_traits>

#include "rod/Singleton.hpp"




struct Component1
{};

struct Component2
{};

struct Component3
{};

struct Component4
{};

struct Component5
{};


void
test()
{
	using level1 = rod::CreateInitialContext<
							rod::Singleton< Component1 >
						>::r;
	using level2 = level1::CreateChildContext<
							rod::Singleton< Component2 >
						>::r;
	using level3 = level2::CreateChildContext<
							rod::Singleton< Component3 >
						>::r;
	using level4 = level3::CreateChildContext<
							rod::Singleton< Component4 >
						>::r;
	using level5 = level4::CreateChildContext<
							rod::Singleton< Component5 >
						>::r;

	static_assert( std::is_same<
						level5::FindOwningContext< Component3 >::r,
						rod::Context<
							rod::CreateContextLevel< rod::Singleton< Component3 > >::r,
							rod::CreateContextLevel< rod::Singleton< Component2 > >::r,
							rod::CreateContextLevel< rod::Singleton< Component1 > >::r
						>
				   >::value,
				   "Unexpected Child Context structure" );
}