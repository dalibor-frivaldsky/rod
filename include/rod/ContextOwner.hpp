#pragma once


#include <utility>

#include <rod/Context.hpp>
#include <rod/ContextAccessor.hpp>




namespace rod
{

	template< typename Ctx >
	struct ContextOwner;

	template<>
	struct ContextOwner< Context<> >
	{
		friend class ContextAccessor< ContextOwner< Context<> > >;


	private:
		Context<>		context;

		inline
		Context<>&
		getContext()
		{
			return context;
		}

		struct GetContext
		{
			using r = Context<>;
		};
	};

	template< typename Context >
	struct ContextOwner
	{
	private:
		using This = ContextOwner< Context >;

		friend class ContextAccessor< This >;


		Context		context;

		Context&
		getContext()
		{
			return context;
		}

		struct GetContext
		{
			using r = Context;
		};


	public:
		ContextOwner( ContextOwner< typename ContextAccessor< This >::Context::ParentContext >& parent ):
		  context( accessContext( parent ) )
		{}

		ContextOwner( const This& other ):
		  context( other.context )
		{}

		ContextOwner( This&& other ):
		  context( std::move( other.context ) )
		{}
	};
	
}