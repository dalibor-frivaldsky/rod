#include <cassert>

#include <rod/Singleton.hpp>
#include <rod/Container.hpp>

#include "../../MockContext.hpp"




int order = 0;

struct FirstToDestroy
{
	~FirstToDestroy()
	{
		assert( order == 0 );
		++order;
	}
};

struct SecondToDestroy
{
	~SecondToDestroy()
	{
		assert( order == 1 );
		++order;
	}
};

struct ThirdToDestroy
{
	~ThirdToDestroy()
	{
		assert( order == 2 );
		++order;
	}
};


void
test()
{
	using c = rod::CreateContainer<
					rod::Singleton< ThirdToDestroy >,
					rod::Singleton< SecondToDestroy >,
					rod::Singleton< FirstToDestroy >
			  >::r;

	auto	mock = mockContext();
	c	cont( mock, std::make_tuple() );
}