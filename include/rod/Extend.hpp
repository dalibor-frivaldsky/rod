#pragma once


#include <type_traits>
#include <utility>

#include <rod/Context.hpp>
#include <rod/ContextAccessor.hpp>
#include <rod/ContextOwner.hpp>




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

		template< typename Parent, typename NewTypes >
		struct ContextBuilder;

		template< typename Parent, typename... NewType >
		struct ContextBuilder< Parent, TypeList< NewType... > >
		{
		private:
			using BuiltOwner = typename Extend< Parent >
									::template With< NewType... >::r;


			Parent&		parent;


		public:
			ContextBuilder( Parent& parent ):
			  parent( parent )
			{}

			BuiltOwner
			operator () ()
			{
				return BuiltOwner( parent );
			}

			template< typename... Type >
			ContextBuilder<
				Parent,
				TypeList< NewType..., Type... > >
			with()
			{
				return ContextBuilder<
							Parent,
							TypeList< NewType..., Type... > >( parent );
			}
		};
		
	}


	template< typename Parent >
	extendDetail::ContextBuilder< Parent, TypeList<> >
	extend( Parent& parent )
	{
		return extendDetail::ContextBuilder< Parent, TypeList<> >( parent );
	}
	
}