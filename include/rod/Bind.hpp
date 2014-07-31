#pragma once


#include <rod/ContextualAccessor.hpp>
#include <rod/ToBeInjected.hpp>
#include <rod/TypeList.hpp>




namespace rod
{


	template<
		typename Parent,
		template< typename > class ToBind,
		typename ToInject = TypeList<>,
		typename ToAdd = TypeList<> >
	struct Bind;


	template<
		typename Parent,
		template< typename > class ToBind,
		typename... ToInject,
		typename... ToAdd >
	struct Bind<
			Parent,
			ToBind,
			TypeList< ToInject... >,
			TypeList< ToAdd... > >
	{
	private:
		template< typename T >
		struct MakeToBeInjected
		{
			using r = ToBeInjected< T >;
		};


		using accessor = ContextualAccessor< Parent >;
		using injectContext = typename accessor::Context::template CreateChildContext<
											typename MakeToBeInjected< ToInject >::r... >::r;
		using interimContext = typename injectContext::template Enrich< ToAdd... >::r;


	public:
		using r = ToBind< interimContext >;

		template< typename... NewToAdd >
		struct Add
		{
			using r = Bind<
						Parent,
						ToBind,
						TypeList< ToInject... >,
						TypeList< ToAdd..., NewToAdd... > >;
		};

		template< typename... NewToInject >
		struct Inject
		{
			using r = Bind<
						Parent,
						ToBind,
						TypeList< ToInject..., NewToInject... >,
						TypeList< ToAdd... > >;
		};
	};
	
}