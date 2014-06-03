#pragma once


#include <rod/annotation/Component.hpp>
#include <rod/holder/ObjectOwner.hpp>
#include <rod/holder/ObjectReference.hpp>




namespace rod
{

	template< typename Type >
	struct ToBeInjected;

	template< typename Type_ >
	struct ToBeInjected< Type_& >
	{
		using Component = annotation::Component;

		using Type = Type_;
		using Holder = holder::ObjectReference< Type_ >;
	};

	template< typename Type_ >
	struct ToBeInjected< Type_&& >
	{
		using Component = annotation::Component;

		using Type = Type_;
		using Holder = holder::ObjectOwner< Type_ >;
	};

	template< typename Type_ >
	struct ToBeInjected
	{
		using Component = annotation::Component;

		using Type = Type_;
		using Holder = holder::ObjectOwner< Type_ >;
	};
	
}