#include <cassert>
#include <type_traits>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "Common.hpp"




template< typename Holder >
struct Selector;

template< template< typename > class Holder >
struct Selector< Holder< Component2 > >
{
	enum { r = true };
};

template< typename Holder >
struct Selector
{
	enum { r = false };
};


void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< Component >,
					rod::Singleton< Component2 >
			  >::r;

	static_assert( std::is_same<
							c::Select< Selector >::r,
							rod::TypeList< Component2 >
							>::value, "Unexpected Select result" );
}