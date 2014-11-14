#pragma once


#include <memory>
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


		std::unique_ptr< Context >		context;

		Context&
		getContext()
		{
			return *context;
		}

		struct GetContext
		{
			using r = Context;
		};


	public:
		template< typename Parent, typename... ToInject >
		ContextOwner( Parent& parent,
					  ToInject&&... toInject ):
		  context( new Context( accessContext( parent ), std::forward< ToInject >( toInject )... ) )
		{}

		ContextOwner( const This& ) = delete;

		ContextOwner( This&& other ):
		  context( std::move( other.context ) )
		{}
	};
	
}