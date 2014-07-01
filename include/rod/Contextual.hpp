#pragma once


#include <exception>
#include <functional>
#include <tuple>
#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/AsSingleton.hpp>
#include <rod/Context.hpp>
#include <rod/ContextualAccessor.hpp>
#include <rod/Generate.hpp>
#include <rod/ToBeInjected.hpp>
#include <rod/TypeList.hpp>
#include <rod/annotation/AutoResolve.hpp>
#include <rod/annotation/ContextualRecord.hpp>
#include <rod/common/TypeName.hpp>




namespace rod
{

	namespace contextual
	{

		template< template< typename > class ToCreate, typename Parent, typename ToInject, typename ToAddList >
		struct BindContextual;

		template< template< typename > class ToCreate, typename Parent, typename... ToInject, typename... ToAdd >
		struct BindContextual< ToCreate, Parent, TypeList< ToInject... >, TypeList< ToAdd... > >
		{
		private:
			using injectContext = typename Parent::Ctx::template CreateChildContext< ToInject... >::r;
			using interimContext = typename injectContext::template Enrich< ToAdd... >::r;

		public:
			using r = ToCreate< interimContext >;
		};


		template< typename... ToInject >
		struct AsToInjectedComponentList
		{
		private:

			template< typename T >
			struct MakeToBeInjected
			{
				using r = ToBeInjected< T >;
			};


		public:
			using r = typename TypeList< ToInject... >::template Apply< MakeToBeInjected >::r;
		};

	}


	template< typename InterimContext, typename... NewType >
	class Contextual
	{
		template< typename BoundContextual >
		friend class ContextualAccessor;


	private:

		using This = Contextual< InterimContext, NewType... >;


	public:

		using ContextualBase = This;
		using Ctx = typename InterimContext::template Enrich< NewType... >::r;
		using ParentContext = typename Ctx::ParentContext;


	private:

		Ctx	context;


	public:


		template< template< typename > class Selector >
		using FindRegisteredType = typename Ctx::template FindRegisteredType< Selector >;


		template< typename... ToInject >
		Contextual( typename Ctx::ParentContext& parentContext, ToInject&&... toInject ):
		  context( parentContext, std::forward< ToInject >( toInject )... )
		{}

		

		template< template< typename > class ToCreate, typename ToAddList, typename... ToInject >
		typename contextual::BindContextual<
					ToCreate,
					This,
					typename contextual::AsToInjectedComponentList< ToInject... >::r,
					ToAddList >::r
		create( ToInject&&... toInject )
		{
			using toInjectList = typename contextual::AsToInjectedComponentList< ToInject... >::r;
			using toCreate = typename contextual::BindContextual< ToCreate, This, toInjectList, ToAddList >::r;

			return toCreate( this->context, std::forward< ToInject >( toInject )... );
		}

		template< template< typename > class ToCreate, typename ToAddList, typename... ToInject >
		typename contextual::BindContextual<
					ToCreate,
					This,
					typename contextual::AsToInjectedComponentList< ToInject... >::r,
					ToAddList >::r*
		createPtr( ToInject&&... toInject )
		{
			using toInjectList = typename contextual::AsToInjectedComponentList< ToInject... >::r;
			using toCreate = typename contextual::BindContextual< ToCreate, This, toInjectList, ToAddList >::r;

			return new toCreate( this->context, std::forward< ToInject >( toInject )... );
		}

		template< typename ToResolve >
		ToResolve
		resolve()
		{
			return context.template resolve< ToResolve >();
		}
	};


	// MSVC2013 internal compiler error if ToAdd... passed as parameter pack to Contextual::create
	// workaround - wrap the ToAdd inside TypeList to avoid passing parameter pack into Contextual::create
	template< template< typename > class ToCreate, typename... ToAdd, typename Parent, typename... ToInject >
	auto
	create( Parent* parent, ToInject&&... toInject )
		-> decltype( parent->template create< ToCreate, TypeList< ToAdd... > >( std::forward< ToInject >( toInject )... ) )
	{
		return parent->template create< ToCreate, TypeList< ToAdd... > >( std::forward< ToInject >( toInject )... );
	}

	template< template< typename > class ToCreate, typename... ToAdd, typename Parent, typename... ToInject >
	auto
	createPtr( Parent* parent, ToInject&&... toInject )
		-> decltype( parent->template createPtr< ToCreate, TypeList< ToAdd... > >() )
	{
		return parent->template createPtr< ToCreate, TypeList< ToAdd... > >( std::forward< ToInject >( toInject )... );
	}

	template< typename ToResolve, typename BindingContextual >
	ToResolve
	resolve( BindingContextual* bindingContextual )
	{
		return bindingContextual->template resolve< ToResolve >();
	}


#define ROD_Contextual_Constructor_Inherit( cls )	using cls< Context >::ContextualBase::Contextual;

#define ROD_Contextual_Constructor_Define( cls )	\
	template< typename... ToInject > \
	cls( typename cls< Context >::ParentContext& parentContext, ToInject&&... toInject ): \
	  cls< Context >::ContextualBase( parentContext, std::forward< ToInject >( toInject )... ) \
	{}

#if defined( __GNUC__ )
	#if __GNUC__ == 4 && __GNUC_MINOR__ >= 8
		#define ROD_Contextual_Constructor( cls )	ROD_Contextual_Constructor_Inherit( cls )
	#else
		#define ROD_Contextual_Constructor( cls )	ROD_Contextual_Constructor_Define( cls )
	#endif
#elif defined( _MSC_VER )
	#define ROD_Contextual_Constructor( cls )	ROD_Contextual_Constructor_Define( cls )
#else
	#define ROD_Contextual_Constructor( cls )	ROD_Contextual_Constructor_Inherit( cls )
#endif


// MSVC2013 does not accept "template" there, nor does it accept rod::resolve< cls >( this )
// when initializing members outside of the constructor initialization list
#if defined( __GNUC__ )
	#define ROD_Resolve( cls )	rod::resolve< cls >( this )
#elif defined( _MSC_VER )
	#define ROD_Resolve( cls )	this->resolve< cls >()
#else
	#define ROD_Resolve( cls )	rod::resolve< cls >( this )
#endif



	/*namespace contextual
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
	}*/
	
}