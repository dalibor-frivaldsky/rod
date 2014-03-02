#pragma once


#include <exception>
#include <functional>
#include <tuple>
#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/AsSingleton.hpp>
#include <rod/Context.hpp>
#include <rod/Generate.hpp>
#include <rod/ToBeInjected.hpp>
#include <rod/annotation/AutoResolve.hpp>
#include <rod/annotation/ContextualRecord.hpp>
#include <rod/common/TypeName.hpp>




namespace rod
{

	namespace contextual
	{
		template< typename Component >
		struct MakeToBeInjected
		{
			using r = ToBeInjected< Component >;
		};

		template< typename Component >
		struct MakeSingleton;

		template< typename Component >
		struct MakeSingleton< AsSingleton< Component > >
		{
			using r = AsSingleton< Component >;
		};

		template< template< typename > class ContextualType >
		struct MakeSingleton< AsContextual< ContextualType > >
		{
			using r = AsContextual< ContextualType >;
		};

		template< typename Component >
		struct MakeSingleton
		{
			using r = AsSingleton< Component >;
		};
	}


	template< typename InterimContext, typename NewTypes = TypeList<> >
	class Contextual
	{

	private:

		using ParentContext = typename InterimContext::ParentContext;
		using Context = typename InterimContext::template Extend< typename NewTypes::template Apply< contextual::MakeSingleton >::r >::r;

		
		Context		context;
		

	public:

		template< template< typename > class Selector >
		using FindAnnotatedContextual = typename Context::template FindAnnotatedContextual< Selector >;

		template< template< typename > class Selector >
		using FindAnnotated = typename Context::template FindAnnotated< Selector >;

		template< typename... ContextualComponent >
		void
		injectContextualComponents( ContextualComponent&&... contextualComponent )
		{
			this->context.injectComponents( std::forward< ContextualComponent >( contextualComponent )... );
		}

		void
		bindToParentContext( ParentContext& parentContext )
		{
			this->context.bindToParent( parentContext );
		}

		template< template< typename > class ToBind >
		ToBind< typename Context::template CreateChildContext< TypeList<> >::r >
		bind()
		{}

		template< template< typename > class ToCreate, typename... ContextualComponent >
		std::unique_ptr<
			ToCreate<
				typename Context::template CreateChildContext<
							typename TypeList< ContextualComponent... >::template Apply< contextual::MakeToBeInjected >::r >::r > >
											   // TODO is unique_ptr correct?
											   // What if someone wants to share it?
											   // What if someone doesn't?
											   // How about value type with move semantics?
		create( ContextualComponent&&... contextualComponent )
		{
			using componentsToBeInjected =
						typename TypeList< ContextualComponent... >::template Apply< contextual::MakeToBeInjected >::r;
			using interimContext = typename Context::template CreateChildContext< componentsToBeInjected >::r;
			using toCreate = ToCreate< interimContext >;

			std::unique_ptr< toCreate >	object( new toCreate );
			object->injectContextualComponents( std::forward< ContextualComponent >( contextualComponent )... );
			object->bindToParentContext( this->context );

			return object;
		}

		template< typename ToCreate >
		std::unique_ptr< ToCreate > // TODO is unique_ptr correct?
									// What if someone wants to share it?
									// What if someone doesn't?
									// How about value type with move semantics?
		create()
		{
			std::unique_ptr< ToCreate >	object( new ToCreate );
			object->bindToParentContext( this->context );

			return object;
		}

		template< typename ToResolve >
		ToResolve&
		resolve()
		{
			return context.template resolve< ToResolve >();
		}

	};


	template< template< typename > class ToBind, typename BindingContextual >
	auto
	bind( BindingContextual* bindingContextual )
		-> decltype( bindingContextual->template bind< ToBind >() )
	{}

	template< template< typename > class ToCreate, typename BoundContextual, typename... ContextualComponent >
	auto
	create( BoundContextual* boundContextual, ContextualComponent&&... contextualComponent )
		-> decltype( boundContextual->template create< ToCreate >( std::forward< ContextualComponent >( contextualComponent )... ) )
	{
		return boundContextual->template create< ToCreate >( std::forward< ContextualComponent >( contextualComponent )... );
	}

	template< typename ToResolve, typename BindingContextual >
	ToResolve&
	resolve( BindingContextual* bindingContextual )
	{
		return bindingContextual->template resolve< ToResolve >();
	}
	
}