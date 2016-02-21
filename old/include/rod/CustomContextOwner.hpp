#pragma once


#include <memory>
#include <type_traits>

#include <rod/ContextAccessor.hpp>
#include <rod/Resolve.hpp>
#include <rod/TypeList.hpp>
#include <rod/annotation/ExtendWith.hpp>
#include <rod/annotation/Requires.hpp>
#include <rod/common/NullType.hpp>
#include <rod/trait/IsContextMovable.hpp>




namespace rod
{

	template< typename Context, template< typename > class CustomContext >
	struct CustomContextOwner;


	namespace customContextOwnerDetail
	{

		template<
			typename Context,
			template< typename > class CustomContext >
		struct BindCustomContext
		{
			using r = CustomContext<
						CustomContextOwner<
							Context,
							CustomContext > >;
		};


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
		struct ContextPointerHolder
		{
		private:
			using This = ContextPointerHolder< Context >;

			std::unique_ptr< Context >	context;


		public:
			template< typename ParentContext, typename... ToInject >
			ContextPointerHolder( ParentContext& parentContext, ToInject&&... toInject ):
			  context( new Context( parentContext, std::forward< ToInject >( toInject )... ) )
			{}

			ContextPointerHolder( const This& ) = delete;

			ContextPointerHolder( This&& other ):
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


		/*template< typename Context >
		struct ContextValueHolder
		{
		private:
			using This = ContextValueHolder< Context >;

			Context	context;


		public:
			template< typename ParentContext, typename... ToInject >
			ContextValueHolder( ParentContext& parentContext, ToInject&&... toInject ):
			  context( parentContext, std::forward< ToInject >( toInject )... )
			{}

			ContextValueHolder( const This& ) = delete;

			ContextValueHolder( This&& other ):
			  context( std::move( other.context ) )
			{}


			Context&
			getContext()
			{
				return context;
			}

			struct GetContext
			{
				using r = Context;
			};
		};*/


		template< typename Context, template< typename > class CustomContext >
		struct BuildContextHolder
		{
		private:
			using BoundCustomContext = typename BindCustomContext< Context, CustomContext >::r;
			using Requirements = typename annotation::GetRequirements< BoundCustomContext >::r;
			using EnrichedContext = typename EnrichContext< 
										Context,
										CustomContext >::r;


		public:
			using r = typename std::conditional<
						trait::IsContextMovable< BoundCustomContext, Requirements >::r,
						ContextPointerHolder< EnrichedContext >,
						ContextPointerHolder< EnrichedContext > >::type;
		};


		template< typename BoundCustomContext, typename Requirements, typename ContextMovable = void >
		struct CustomContextHolder;

		template< typename BoundCustomContext, typename... Requirement >
		struct CustomContextHolder<
				BoundCustomContext,
				TypeList< Requirement... >,
				typename std::enable_if<
					trait::IsContextMovable<
						BoundCustomContext,
						TypeList< Requirement... > >::r >::type >:
			public BoundCustomContext
		{
		private:
			using This = CustomContextHolder< BoundCustomContext, TypeList< Requirement... > >;


		public:
			template< typename Context >
			CustomContextHolder( Context& context ):
			  BoundCustomContext( context, resolve< Requirement >( context ) ... )
			{}

			CustomContextHolder( const This& ) = delete;
			CustomContextHolder( This&& ) = delete;

			template< typename Context >
			CustomContextHolder( This&& other, Context& context ):
			  BoundCustomContext(
			  	static_cast< BoundCustomContext&& >( std::move( other ) ),
			  	context,
			  	resolve< Requirement >( context )... )
			{}
		};

		template< typename BoundCustomContext, typename... Requirement, typename ContextMovable >
		struct CustomContextHolder<
				BoundCustomContext,
				TypeList< Requirement... >,
				ContextMovable >:
			public BoundCustomContext
		{
		private:
			using This = CustomContextHolder< BoundCustomContext, TypeList< Requirement... > >;


		public:
			template< typename Context >
			CustomContextHolder( Context& context ):
			  BoundCustomContext( context, resolve< Requirement >( context ) ... )
			{}

			CustomContextHolder( const This& ) = delete;
			CustomContextHolder( This&& ) = delete;

			template< typename Context >
			CustomContextHolder( This&& other, Context& ):
			  BoundCustomContext( static_cast< BoundCustomContext&& >( std::move( other ) ) )
			{}
		};


		template< typename Context, template< typename > class CustomContext >
		struct BuildCustomContextHolder
		{
		private:
			using BoundCustomContext = typename BindCustomContext< Context, CustomContext >::r;
			using Requirements = typename annotation::GetRequirements< BoundCustomContext >::r;

		public:
			using r = CustomContextHolder< BoundCustomContext, Requirements >;
		};
		
	}


	template< typename Context, template< typename > class CustomContext >
	struct CustomContextOwner:
		private customContextOwnerDetail::BuildContextHolder<
					Context, CustomContext >::r,
		public customContextOwnerDetail::BuildCustomContextHolder<
					Context, CustomContext >::r
	{
	private:
		using This = CustomContextOwner< Context, CustomContext >;
		using ContextHolder = typename customContextOwnerDetail::BuildContextHolder<
								Context, CustomContext >::r;
		using CustomContextHolder = typename customContextOwnerDetail::BuildCustomContextHolder<
											Context, CustomContext >::r;

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
		  CustomContextHolder( *this )
		{}

		CustomContextOwner( const This& ) = delete;

		CustomContextOwner( This&& other ):
		  ContextHolder( static_cast< ContextHolder&& >( std::move( other ) ) ),
		  CustomContextHolder( static_cast< CustomContextHolder&& >( std::move( other ) ), *this )
		{}
	};
	
}