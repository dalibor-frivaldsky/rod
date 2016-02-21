#pragma once


#include <utility>




namespace rod
{

	namespace common
	{

		template< size_t Count >
		struct RotateArgs;

		template<>
		struct RotateArgs< 0 >
		{
			template< typename Closure, typename... Arg >
			static
			void
			rotate( Closure&& closure, Arg&&... arg )
			{
				closure( std::forward< Arg >( arg )... );
			}
		};

		template< size_t Count >
		struct RotateArgs
		{
			template< typename Closure, typename First, typename... Rest >
			static
			void
			rotate( Closure&& closure, First&& first, Rest&&... rest )
			{
				RotateArgs< Count - 1 >::rotate(
					std::forward< Closure >( closure ),
					std::forward< Rest >( rest )...,
					std::forward< First >( first ) );
			}
		};
		
	}
	
}