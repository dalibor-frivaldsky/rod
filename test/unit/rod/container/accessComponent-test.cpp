#include <cassert>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "Common.hpp"
#include "../../MockContext.hpp"




void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< Component >,
					rod::Singleton< Component2 >
			  >::r;

	auto	mock = mockContext();
	c	cont( mock, std::make_tuple() );

	assert( cont.access< Component >().get().method( 4 ) == 4 );
}