#pragma once


#include <tuple>
#include <type_traits>
#include <utility>

#include <rod/Context.hpp>
#include <rod/ContextAccessor.hpp>
#include <rod/ContextOwner.hpp>
#include <rod/Injected.hpp>
#include <rod/common/Sequence.hpp>




namespace rod
{

	namespace extendDetail
	{

		template< typename Parent, typename NewTypes >
		struct ContextExtender;


		template< typename Extender, typename... Type >
		struct AddTypes;

		template< typename Parent, typename NewTypes, typename... Type >
		struct AddTypes< ContextExtender< Parent, NewTypes >, Type... >
		{
			using r = ContextExtender<
							Parent,
							typename NewTypes::template Append< Type... >::r >;
		};


		template< typename Parent, typename... NewType >
		struct ContextExtender< Parent, TypeList< NewType... > >
		{
		private:
			using This = ContextExtender< Parent, TypeList< NewType... > >;

			using ParentContext = typename ContextAccessor<
									typename std::remove_reference< Parent >::type >::Context;

		public:
			using r = ContextOwner< typename ParentContext::template CreateChildContext< NewType... >::r >;

			template< typename... Type >
			using With = typename AddTypes< This, Type... >::r;
		};
		
	}


	template< typename Parent >
	using Extend = extendDetail::ContextExtender< Parent, TypeList<> >;



	namespace extendDetail
	{

		template< typename ToBuild >
		struct OwnerBuilder
		{
			template< typename Parent, typename ToInjectTuple, int... Seq >
			static
			ToBuild
			build( Parent& parent, ToInjectTuple& toInjectTuple, common::Sequence< Seq... >&& )
			{
				return ToBuild( parent, std::get< Seq >( toInjectTuple )... );
			}
		};


		template< typename Parent, typename NewTypes, typename ToInjectTypes >
		struct ContextBuilder;

		template< typename Parent, typename... NewType, typename... ToInjectType >
		struct ContextBuilder< Parent, TypeList< NewType... >, TypeList< ToInjectType... > >
		{
		private:
			using BuiltOwner = typename Extend< Parent >
									::template With< Injected< ToInjectType >..., NewType... >::r;


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
				TypeList< ToInjectType..., ToInject... > >
			with( ToInject&&... toInject )
			{
				return ContextBuilder<
							Parent,
							TypeList< NewType..., Type... >,
							TypeList< ToInjectType..., ToInject... > >(
								parent,
								std::tuple_cat(
									toInjectTuple,
									std::tuple< ToInject... >( std::forward< ToInject >( toInject )... ) ) );
			}
		};
		
	}


	template< typename Parent >
	extendDetail::ContextBuilder< Parent, TypeList<>, TypeList<> >
	extend( Parent& parent )
	{
		return extendDetail::ContextBuilder<
				Parent,
				TypeList<>,
				TypeList<> >(
					parent,
					std::tuple<>() );
	}
	
}