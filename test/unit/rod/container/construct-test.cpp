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

	c	cont( std::make_tuple() );
}