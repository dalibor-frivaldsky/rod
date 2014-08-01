#include <cassert>

#include <rod/AsSingleton.hpp>
#include <rod/Container.hpp>




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
					rod::AsSingleton< ThirdToDestroy >,
					rod::AsSingleton< SecondToDestroy >,
					rod::AsSingleton< FirstToDestroy >
			  >::r;

	c	cont(  std::make_tuple() );
}