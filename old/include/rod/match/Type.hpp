#pragma once




namespace rod
{

	namespace match
	{

		namespace detail
		{

			template< typename Type, typename T >
			struct TypeMatcher;

			template< typename Type >
			struct TypeMatcher< Type, Type >
			{
				enum { r = true };
			};

			template< typename Type, typename T >
			struct TypeMatcher
			{
				enum { r = false };
			};
			
		}

		template< typename ToMatchType >
		struct Type
		{
			template< typename T >
			using Match = detail::TypeMatcher< ToMatchType, T >;
		};
	
	}
	
}
