#pragma once


#include <rod/annotation/Component.hpp>
#include <rod/holder/SingletonHolder.hpp>




namespace rod
{

	template< typename Type_ >
	struct AsSingleton
	{
		using Component = annotation::Component;

		using Type = Type_;
		using Holder = holder::SingletonOwner< Type_ >;
	};
	
}