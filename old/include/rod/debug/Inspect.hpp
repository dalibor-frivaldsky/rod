#pragma once




namespace rod
{

	namespace debug
	{

		namespace detail
		{

			template< typename T >
			struct Print
			{};
			
		}

		template< typename T >
		struct Inspect
		{
			using r = typename detail::Print< T >::inspect;
		};
		
	}
	
}