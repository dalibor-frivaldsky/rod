#pragma once


#include <rod/holder/SingletonHolder.hpp>




namespace rod
{

	template< typename Component >
	struct AsSingleton
	{
		using Type = Component;
		using Holder = holder::SingletonOwner< Component >;
	};
	
}