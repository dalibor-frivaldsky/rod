#pragma once


#include <functional>
#include <utility>

#include <rod/ContextAccessor.hpp>




namespace rod
{

	namespace impl
	{

		template< typename LambdaOp >
		struct WithDeducedDeps;

		template< typename Class, typename Return, typename... Dep >
		struct WithDeducedDeps< Return (Class::*)( Dep... ) const >
		{
			template< typename Context, typename Closure >
			static
			void
			perform( Context& context, Closure&& closure )
			{
				closure( accessContext( context ).template resolve< Dep >()... );
			}
		};
		
	}

	template< typename Context, typename Closure >
	void
	with( Context& context, Closure&& closure )
	{
		impl::WithDeducedDeps< decltype( &Closure::operator() ) >::perform( context, std::forward< Closure >( closure ) );
	}
	
}