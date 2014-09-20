#include <cassert>
#include <type_traits>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "Common.hpp"




void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< Component >,
					rod::Singleton< Component2 >
			  >::r;

	static_assert( std::is_same<
							c::Access< Component2 >::r,
							rod::holder::SingletonOwner< Component2 >
							>::value, "Unexpected Component Holder" );
}