#include <cassert>
#include <type_traits>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "Common.hpp"




void
test()
{
	using c1 = rod::CreateContainer<
					rod::Singleton< Component >
			  >::r;

	using c2 = rod::CreateContainer<
					rod::Singleton< Component2 >
			  >::r;

	using merged = c1::Merge< c2 >::r;

	static_assert( std::is_same<
							merged,
							rod::Container<
								rod::container::ComponentHolder<
									Component,
									rod::holder::SingletonOwner< Component >
								>,
								rod::container::ComponentHolder<
									Component2,
									rod::holder::SingletonOwner< Component2 >
								>
							> >::value, "Unexpected Merge result" );
}