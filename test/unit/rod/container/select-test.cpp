#include <cassert>
#include <type_traits>

#include <rod/AsSingleton.hpp>
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
					rod::AsSingleton< Component >,
					rod::AsSingleton< Component2 >
			  >::r;

	static_assert( std::is_same<
							c::Select< Selector >::r,
							rod::TypeList< Component2 >
							>::value, "Unexpected Select result" );
}