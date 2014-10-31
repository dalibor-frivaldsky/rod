#pragma once


#include <rod/ContextAccessor.hpp>
#include <rod/Find.hpp>
#include <rod/Generate.hpp>
#include <rod/TypeList.hpp>
#include <rod/match/Component.hpp>
#include <rod/match/Interface.hpp>




namespace rod
{

	namespace detail
	{

		template< typename Implementers >
		struct Perform;

		// MSVC2013 has issues with zero implementers
		template<>
		struct Perform< TypeList<> >
		{
			template< typename Context, typename Closure >
			static
			void
			perform( Context&, Closure&& )
			{}
		};

		template< typename... Implementer >
		struct Perform< TypeList< Implementer... > >
		{
			template< typename Context, typename Closure >
			static
			void
			perform( Context& context, Closure&& closure )
			{
				rod::generate
				{
					(closure( accessContext( context ).template resolve< Implementer& >() ), 0)...
				};
			}
		};
		
	}


	template< typename Interface, typename Context, typename Closure >
	void
	each( Context& context, Closure&& closure )
	{
		using Implementers = typename Find<
								Context,
								match::Component< match::Interface< Interface > > >::r;

		detail::Perform< Implementers >::perform( context, closure );
	}
	
}