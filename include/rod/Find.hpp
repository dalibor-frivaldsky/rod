#pragma once


#include <type_traits>

#include <rod/ContextAccessor.hpp>
#include <rod/annotation/Component.hpp>




namespace rod
{

	namespace detail
	{

		template< typename Type, typename IsComponent = void >
		struct GetTypeImpl;

		template< typename Component >
		struct GetTypeImpl<
			Component,
			typename std::enable_if< annotation::IsComponent< Component >::r >::type >
		{
			using r = typename Component::Type;
		};

		template< typename Type, typename IsNotComponent >
		struct GetTypeImpl
		{
			using r = Type;
		};


		template< typename Type >
		struct GetType
		{
			using r = typename GetTypeImpl< Type >::r;
		};
		
	}

	template< typename Context, typename Matcher >
	struct Find
	{
	private:
		using Types = typename ContextAccessor< Context >::Context::GetTypeRegistry::r;

	public:
		using r = typename Types::template Find< Matcher::template Match >::r
						::template Apply< detail::GetType >::r;
	};
	
}