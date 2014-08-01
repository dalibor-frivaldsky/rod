#pragma once


#include <functional>
#include <utility>

#include <rod/ContextualAccessor.hpp>




namespace rod
{

	namespace impl
	{

		template< typename LambdaOp >
		struct WithDeducedDeps;

		template< typename Class, typename Return, typename... Dep >
		struct WithDeducedDeps< Return (Class::*)( Dep... ) const >
		{
			template< typename Entry, typename Lambda >
			static
			void
			perform( Entry* entry, Lambda&& lambda )
			{
				ContextualAccessor< Entry >	accessor( entry );

				lambda( accessor.context().template resolve< Dep >()... );
			}
		};
		
	}

	template< typename Entry, typename Lambda >
	void
	with( Entry* entry, Lambda&& lambda )
	{
		impl::WithDeducedDeps< decltype( &Lambda::operator() ) >::perform( entry, std::forward< Lambda >( lambda ) );
	}
	
}