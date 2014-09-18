#pragma once


#include <rod/ContextAccessor.hpp>




namespace rod
{

	template< typename Context, typename Matcher >
	struct Find
	{
	private:
		using Types = typename ContextAccessor< Context >::Context::GetTypeRegistry::r;

	public:
		using r = typename Types::template Find< Matcher::template Match >::r;
	};


	namespace isTypeDetail
	{

		template< typename Type, typename T >
		struct IsTypeMatcher;

		template< typename Type >
		struct IsTypeMatcher< Type, Type >
		{
			enum { r = true };
		};

		template< typename Type, typename T >
		struct IsTypeMatcher
		{
			enum { r = false };
		};
		
	}

	template< typename Type >
	struct IsType
	{
		template< typename T >
		using Match = isTypeDetail::IsTypeMatcher< Type, T >;
	};
	
}