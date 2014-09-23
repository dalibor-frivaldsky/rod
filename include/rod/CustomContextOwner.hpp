#pragma once


#include <memory>

#include <rod/ContextAccessor.hpp>
#include <rod/TypeList.hpp>
#include <rod/annotation/ExtendWith.hpp>
#include <rod/common/NullType.hpp>




namespace rod
{

	namespace customContextOwnerDetail
	{

		template<
			typename Context,
			template< typename > class CustomContext >
		struct EnrichContext
		{
		private:
			using EnrichWith = typename annotation::GetExtendWith<
									CustomContext< common::NullType > >::r;


			template< typename Types >
			struct Enrich;

			template< typename... Type >
			struct Enrich< TypeList< Type... > >
			{
				using r = typename Context::template Enrich< Type... >::r;
			};

		public:
			using r = typename Enrich< EnrichWith >::r;
		};


		template< typename Context >
		struct ContextHolder
		{
		private:
			using This = ContextHolder< Context >;

			std::unique_ptr< Context >	context;


		public:
			template< typename ParentContext, typename... ToInject >
			ContextHolder( ParentContext& parentContext, ToInject&&... toInject ):
			  context( new Context( parentContext, std::forward< ToInject >( toInject )... ) )
			{}

			ContextHolder( const This& ) = delete;

			ContextHolder( This&& other ):
			  context( std::move( other.context ) )
			{}


			Context&
			getContext()
			{
				return *context;
			}

			struct GetContext
			{
				using r = Context;
			};
		};
		
	}


	template< typename Context, template< typename > class CustomContext >
	struct CustomContextOwner:
		private customContextOwnerDetail::ContextHolder<
			typename customContextOwnerDetail::EnrichContext<
				Context, CustomContext >::r >,
		public CustomContext<
			CustomContextOwner<
				Context, CustomContext > >
	{
	private:
		using This = CustomContextOwner< Context, CustomContext >;
		using ContextHolder = customContextOwnerDetail::ContextHolder<
								typename customContextOwnerDetail::EnrichContext<
									Context, CustomContext >::r >;
		using BoundCustomContext = CustomContext< This >;

		friend class ContextAccessor< This >;


		typename ContextHolder::GetContext::r&
		getContext()
		{
			return ContextHolder::getContext();
		}

		struct GetContext
		{
			using r = typename ContextHolder::GetContext::r;
		};


	public:
		template< typename Parent, typename... ToInject >
		CustomContextOwner( Parent& parent,
							ToInject&&... toInject ):
		  ContextHolder( accessContext( parent ), std::forward< ToInject >( toInject )... ),
		  BoundCustomContext( *this )
		{}

		CustomContextOwner( const This& ) = delete;

		CustomContextOwner( This&& other ):
		  ContextHolder( static_cast< ContextHolder&& >( std::move( other ) ) ),
		  BoundCustomContext( static_cast< BoundCustomContext&& >( std::move( other ) ) )
		{}
	};
	
}