#pragma once


#include <type_traits>

#include <rod/annotation/Component.hpp>




namespace rod
{

	namespace match
	{

		namespace detail
		{

			template< typename ComponentTypeMatcher, typename Type, typename IsComponent = void >
			struct ComponentMatcher;

			template< typename ComponentTypeMatcher, typename Component >
			struct ComponentMatcher<
				ComponentTypeMatcher,
				Component,
				typename std::enable_if< annotation::IsComponent< Component >::r >::type >
			{
				enum { r = ComponentTypeMatcher::template Match< typename Component::Type >::r };
			};

			template< typename ComponentTypeMatcher, typename Type, typename IsNotComponent >
			struct ComponentMatcher
			{
				enum { r = false };
			};
			
		}

		template< typename ComponentTypeMatcher >
		struct Component
		{
			template< typename Type >
			using Match = detail::ComponentMatcher< ComponentTypeMatcher, Type >;
		};
		
	}
	
}