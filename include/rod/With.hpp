#pragma once


#include <functional>
#include <utility>

#include <rod/ContextAccessor.hpp>




namespace rod
{

	namespace impl
	{

		template< typename Deps >
		struct WithDeps;

		template< typename ... Dep >
		struct WithDeps< TypeList< Dep... > >
		{
			template< typename Context, typename Closure >
			static
			void
			perform( Context& context, Closure&& closure )
			{
				closure( accessContext( context ).template resolve< Dep >()... );
			}
		};


		template< typename Return, typename Class, typename... Arg >
		TypeList< Arg... >
		getDeps( Return (Class::*)( Arg... ) const )
		{}
		
	}

	template< typename Context, typename Closure >
	void
	with( Context& context, Closure&& closure )
	{
		using Deps = decltype( impl::getDeps( &Closure::operator() ) );
		impl::WithDeps< Deps >::perform( context, std::forward< Closure >( closure ) );
	}
	
}