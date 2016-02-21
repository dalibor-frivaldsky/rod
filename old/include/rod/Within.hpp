#pragma once


#include <utility>

#include <rod/Extend.hpp>
#include <rod/Injected.hpp>
#include <rod/common/RotateArgs.hpp>




namespace rod
{

	namespace detail
	{

		template< typename Context, typename... ToAdd >
		struct WithinClosure
		{
		private:
			Context& context;


		public:
			WithinClosure( Context& context ):
			  context( context )
			{}


			template< typename Closure, typename... Object >
			void operator () ( Closure&& closure, Object&&... object )
			{
				typename Extend< Context >
					::template With<
						ToAdd...,
						Injected< Object >... >::r	extended(
														context,
														std::forward< Object >( object )... );

				closure( extended );
			}
		};
		
	}

	template< typename... ToAdd, typename Context, typename... Object >
	void
	within( Context& context, Object&&... object )
	{
		common::RotateArgs< sizeof...( Object ) - 1 >
			::rotate(
				detail::WithinClosure<
					Context,
					ToAdd... >(
						context ),
				std::forward< Object >( object )... );
	}
	
}