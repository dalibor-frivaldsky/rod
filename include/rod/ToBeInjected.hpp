#pragma once


#include <rod/holder/ObjectOwner.hpp>
#include <rod/holder/ObjectReference.hpp>




namespace rod
{

	template< typename Component >
	struct ToBeInjected;

	template< typename Component >
	struct ToBeInjected< Component& >
	{
		using Type = Component;
		using Holder = holder::ObjectReference< Component >;
	};

	template< typename Component >
	struct ToBeInjected< Component&& >
	{
		using Type = Component;
		using Holder = holder::ObjectOwner< Component >;
	};

	template< typename Component >
	struct ToBeInjected
	{
		using Type = Component;
		using Holder = holder::ObjectOwner< Component >;
	};
	
}