#pragma once


#include <tuple>
#include <type_traits>
#include <utility>

#include <rod/Context.hpp>
#include <rod/ContextAccessor.hpp>
#include <rod/ContextOwner.hpp>
#include <rod/CustomContextOwner.hpp>
#include <rod/Injected.hpp>
#include <rod/common/Sequence.hpp>




namespace rod
{

	namespace extendDetail
	{

		struct NoCustomContext
		{};

		template< template< typename > class CustomContext_ >
		struct CustomContextProvider
		{
			template< typename T >
			using CustomContext = CustomContext_< T >;
		};


		template< typename Parent, typename Types, typename CustomContext >
		struct OwnerAssembler;

		template< typename Parent, typename... Type >
		struct OwnerAssembler< Parent, TypeList< Type... >, NoCustomContext >
		{
		private:
			using ParentContext = typename ContextAccessor<
									typename std::remove_reference< Parent >::type >::Context;


		public:
			using r = ContextOwner<
						typename ParentContext::template CreateChildContext<
							Type... >::r >;
		};

		template< typename Parent, typename... Type, typename CustomContext >
		struct OwnerAssembler< Parent, TypeList< Type... >, CustomContext >
		{
		private:
			using ParentContext = typename ContextAccessor<
									typename std::remove_reference< Parent >::type >::Context;


		public:
			using r = CustomContextOwner<
						typename ParentContext::template CreateChildContext<
							Type... >::r,
						CustomContext::template CustomContext >;
		};


		template< typename Parent, typename NewTypes, typename CustomContext >
		struct ContextExtender;


		template< typename Extender, typename... Type >
		struct AddTypes;

		template< typename Parent, typename NewTypes, typename CustomContext, typename... Type >
		struct AddTypes< ContextExtender< Parent, NewTypes, CustomContext >, Type... >
		{
			using r = ContextExtender<
							Parent,
							typename NewTypes::template Append< Type... >::r,
							CustomContext >;
		};


		template< typename Parent, typename... NewType, typename CustomContext >
		struct ContextExtender< Parent, TypeList< NewType... >, CustomContext >
		{
		private:
			using This = ContextExtender<
							Parent,
							TypeList< NewType... >,
							CustomContext >;
			

		public:
			using r = typename OwnerAssembler<
						Parent,
						TypeList< NewType... >,
						CustomContext >::r;

			template< typename... Type >
			using With = typename AddTypes< This, Type... >::r;

			template< template< typename > class CustomContext_ >
			using As = ContextExtender<
							Parent,
							TypeList< NewType... >,
							CustomContextProvider< CustomContext_ > >;
		};
		
	}


	template< typename Parent >
	using Extend = extendDetail::ContextExtender< Parent, TypeList<>, extendDetail::NoCustomContext >;



	namespace extendDetail
	{

		template< typename ToBuild >
		struct OwnerBuilder
		{
			template< typename Parent >
			static
			ToBuild
			build( Parent& parent, std::tuple<>&, common::Sequence<>&& )
			{
				return ToBuild( parent );
			}

			template< typename Parent, typename ToInjectTuple, int... Seq >
			static
			ToBuild
			build( Parent& parent, ToInjectTuple& toInjectTuple, common::Sequence< Seq... >&& )
			{
				return ToBuild(
						parent,
						std::forward<
							typename std::tuple_element< Seq, ToInjectTuple >::type >(
								std::get< Seq >( toInjectTuple ) )... );
			}
		};


		template< typename Parent, typename NewTypes, typename ToInjectTypes, typename CustomContext >
		struct ContextBuilder;

		template< typename Parent, typename... NewType, typename... ToInjectType, typename CustomContext >
		struct ContextBuilder< Parent, TypeList< NewType... >, TypeList< ToInjectType... >, CustomContext >
		{
		private:
			using BuiltOwner = typename OwnerAssembler<
									Parent,
									TypeList< Injected< ToInjectType >..., NewType... >,
									CustomContext >::r;


			Parent&							parent;
			std::tuple< ToInjectType... >	toInjectTuple;


		public:
			ContextBuilder( Parent& parent, std::tuple< ToInjectType... >&& toInjectTuple ):
			  parent( parent ),
			  toInjectTuple( std::move( toInjectTuple ) )
			{}

			BuiltOwner
			operator () ()
			{
				using	seq = typename common::GenerateSequence< sizeof...( ToInjectType ) >::r;
				return OwnerBuilder< BuiltOwner >::build( parent, toInjectTuple, seq() );
			}

			template< typename... Type, typename... ToInject >
			ContextBuilder<
				Parent,
				TypeList< NewType..., Type... >,
				TypeList< ToInjectType..., ToInject... >,
				CustomContext >
			with( ToInject&&... toInject )
			{
				return ContextBuilder<
							Parent,
							TypeList< NewType..., Type... >,
							TypeList< ToInjectType..., ToInject... >,
							CustomContext >(
								parent,
								std::tuple_cat(
									toInjectTuple,
									std::tuple< ToInject... >( std::forward< ToInject >( toInject )... ) ) );
			}

			template< template< typename > class CustomContext_ >
			ContextBuilder<
				Parent,
				TypeList< NewType... >,
				TypeList< ToInjectType... >,
				extendDetail::CustomContextProvider< CustomContext_ > >
			as()
			{
				return ContextBuilder<
							Parent,
							TypeList< NewType... >,
							TypeList< ToInjectType... >,
							extendDetail::CustomContextProvider< CustomContext_ > >(
								parent,
								std::move( toInjectTuple ) );
			}
		};
		
	}


	template< typename Parent >
	extendDetail::ContextBuilder< Parent, TypeList<>, TypeList<>, extendDetail::NoCustomContext >
	extend( Parent& parent )
	{
		return extendDetail::ContextBuilder<
				Parent,
				TypeList<>,
				TypeList<>,
				extendDetail::NoCustomContext >(
					parent,
					std::tuple<>() );
	}
	
}