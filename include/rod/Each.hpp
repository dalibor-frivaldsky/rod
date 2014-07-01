#pragma once


#include <functional>
#include <type_traits>

#include <rod/ContextualAccessor.hpp>
#include <rod/Generate.hpp>




namespace rod
{

	template< typename Interface, typename Contextual >
	class Each:
		private ContextualAccessor< Contextual >
	{
	private:
		using Base = ContextualAccessor< Contextual >;
		using Closure = std::function< void( Interface& ) >;


		template< typename T >
		struct HasInterface
		{
			enum { r = std::is_base_of< Interface, T >::value };
		};


		template< typename Implementors >
		struct UnpackPerform;

		template< typename... Implementor >
		struct UnpackPerform< TypeList< Implementor... > >
		{
			template< typename Context >
			static
			void
			perform( Context& ctx, Closure& closure )
			{
				rod::generate
				{
					(closure( ctx.template resolve< Implementor& >() ), 0)...
				};
			}
		};


	public:
		Each( Contextual* contextual ):
		  Base( contextual )
		{}


		void
		perform( Closure& closure )
		{
			using	context = typename Base::Context;
			using	implementors = typename context::template FindImplementors< Interface >::r;

			UnpackPerform< implementors >::perform( this->context(), closure );
		}
	};


	template< typename Interface, typename Contextual >
	void
	each( Contextual* contextual, std::function< void( Interface& ) >&& closure )
	{
		Each< Interface, Contextual >( contextual ).perform( closure );
	}

	template< typename Interface, typename Contextual >
	void
	each( Contextual* contextual, std::function< void( Interface& ) >& closure )
	{
		Each< Interface, Contextual >( contextual ).perform( closure );
	}
	
}