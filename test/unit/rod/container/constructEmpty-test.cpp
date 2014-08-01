#include <rod/AsSingleton.hpp>
#include <rod/Container.hpp>

#include "Common.hpp"




void
test()
{
	using c = rod::CreateContainer<>::r;

	c	cont(  std::make_tuple() );
}