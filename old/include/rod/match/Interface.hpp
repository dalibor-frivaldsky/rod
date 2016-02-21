#pragma once


#include <type_traits>




namespace rod
{

	namespace match
	{

		namespace detail
		{

			template< typename Interface, typename Type >
			struct InterfaceMatcher
			{
				enum { r = std::is_base_of< Interface, Type >::value };
			};
			
		}

		template< typename I >
		struct Interface
		{
			template< typename Type >
			using Match = detail::InterfaceMatcher< I, Type >;
		};
		
	}
	
}