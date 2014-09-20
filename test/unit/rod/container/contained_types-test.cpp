#include <rod/Singleton.hpp>
#include <rod/Container.hpp>
#include <rod/TypeList.hpp>

#include "Common.hpp"

#include <type_traits>




void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< Component >,
					rod::Singleton< Component2 >
			  >::r;

	static_assert( std::is_same<
						c::ContainedTypes::r,
						rod::TypeList< Component, Component2 > >::value,
				   "Unexpected contained types" );
}