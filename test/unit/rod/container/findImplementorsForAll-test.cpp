#include <cassert>
#include <type_traits>

#include <rod/AsSingleton.hpp>
#include <rod/Container.hpp>

#include "Common.hpp"




void
test()
{
	using c = rod::CreateContainer<
					rod::AsSingleton< Component >,
					rod::AsSingleton< Component2 >
			  >::r;

	using result = c::FindImplementorsForAll< rod::TypeList< ComponentInterface, Component2Interface > >::r;

	static_assert( std::is_same<
							result,
							rod::TypeList< rod::TypeList< Component >, rod::TypeList< Component2 > >
							>::value, "Unexpected FindImplementorsForAll result" );
}