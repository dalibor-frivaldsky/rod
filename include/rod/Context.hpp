#pragma once


#include <functional>
#include <type_traits>
#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/ContextLevel.hpp>
#include <rod/TypeList.hpp>
#include <rod/annotation/Component.hpp>
#include <rod/annotation/ContextualRecord.hpp>
#include <rod/annotation/Resolver.hpp>
#include <rod/holder/InjectedReference.hpp>
#include <rod/holder/InjectedValue.hpp>




namespace rod
{

	template< typename... ContextLevels >
	struct Context;


	namespace context
	{

		template< typename Ctx, typename... NewType >
		struct CreateChildContext;

		template< typename... CtxLevel, typename... NewType >
		struct CreateChildContext< Context< CtxLevel... >, NewType... >
		{
			using r = Context<
						typename CreateContextLevel< NewType... >::r,
						CtxLevel... >;
		};


		template< typename Context, typename Deps >
		struct CanResolveDeps;

		template< typename Ctx, typename... Dep >
		struct CanResolveDeps< Ctx, TypeList< Dep... > >
		{
		private:
			template< typename Next, typename Result >
			struct AppendIfCannotResolve
			{
				using r = typename std::conditional<
							Ctx::template CanResolve< Next >::r,
							Result,
							typename Result::template Append< Next >::r >::type;
			};

		public:
			enum { r = Reduce<
							AppendIfCannotResolve,
							TypeList<>,
							Dep... >::r::Length::r == 0 };
		};


		template< typename Context, typename... Component >
		struct OrderedLevelEnrich;

		template< typename CurrentLevel, typename... ParentLevel >
		struct OrderedLevelEnrich< Context< CurrentLevel, ParentLevel... > >
		{
			using r = CurrentLevel;
		};

		template< typename CurrentLevel, typename... ParentLevel, typename... Component >
		struct OrderedLevelEnrich< Context< CurrentLevel, ParentLevel... >, Component... >
		{
		private:
			using ctx = Context< CurrentLevel, ParentLevel... >;

			template< typename Next, typename Result >
			struct DepsSatisfier
			{
				using r = typename std::conditional<
							CanResolveDeps< ctx, typename Next::Holder::Dependencies >::r,
							typename Result::template Append< Next >::r,
							Result >::type;
			};

			using satisfiedComponents = typename Reduce<
													DepsSatisfier,
													TypeList<>,
													Component... >::r;

			using unsatisfiedComponents = typename TypeList< Component... >
											::template RemoveList< satisfiedComponents >::r;

			static_assert( satisfiedComponents::Length::r, "Unable to create all components - dependencies not met" );
			
			using enrichedLevel = typename satisfiedComponents
											::template UnpackTo<
												CurrentLevel::template Enrich >::r::r;

			template< typename Ctx >
			struct ApplyToOrderedLevelEnrich
			{
				template< typename... Comp >
				using Apply = OrderedLevelEnrich< Ctx, Comp... >;
			};

		public:
			using r = typename unsatisfiedComponents::template UnpackTo<
									ApplyToOrderedLevelEnrich<
										Context< enrichedLevel, ParentLevel... > >::template Apply >::r::r;
		};


		template< typename... NewType >
		struct FilterComponents
		{
		private:
			template< typename T >
			struct IsNotComponent
			{
				enum { r = annotation::IsComponent< T >::r == false };
			};

		public:
			using Components = typename TypeList< NewType... >::template Select< annotation::IsComponent >::r;
			using NotComponents = typename TypeList< NewType... >::template Select< IsNotComponent >::r;
		};


		template< typename Context, typename... NewType >
		struct Enrich;

		template< typename CurrentLevel, typename... ParentLevel, typename... NewType >
		struct Enrich< Context< CurrentLevel, ParentLevel... >, NewType... >
		{
		private:
			using filtered = FilterComponents< NewType... >;
			using nonComponentEnriched = typename filtered::NotComponents::template UnpackTo<
											CurrentLevel::template Enrich >::r::r;
			using enriched = typename filtered::Components::template UnpackTo1<
										OrderedLevelEnrich,
										Context< nonComponentEnriched, ParentLevel... > >::r::r;

		public:
			using r = Context< enriched, ParentLevel... >;
		};


		template< typename Ctx >
		struct GetTypeRegistry;

		template< typename... CtxLevel >
		struct GetTypeRegistry< Context< CtxLevel... > >
		{
		private:
			template< typename CLevel >
			struct GetRegistry
			{
				using r = typename CLevel::Registry;
			};


			template< typename NextTypeRegistry, typename Result >
			struct MergeRegistryOp;

			template< typename... Type, typename Result >
			struct MergeRegistryOp< TypeRegistry< Type... >, Result >
			{
				using r = typename Result::template Prepend< Type... >::r;
			};


			using mergedTypes = typename Reduce< MergeRegistryOp, TypeList<>, typename GetRegistry< CtxLevel >::r... >::r;


		public:
			using r = typename mergedTypes::template UnpackTo< TypeRegistry >::r;
		};


		template< typename Context, template< typename > class Selector >
		struct FindRegisteredType
		{
			using r = typename Context::GetTypeRegistry::r::template Find< Selector >::r;
		};


		template< typename Context, template< typename > class Selector >
		struct FindRegisteredContextual
		{
		private:

			template< typename ContextualRec >
			struct ContextBinder
			{
				using r = typename ContextualRec::template Bind< Context >::r;
			};


			template< typename BoundContextual >
			struct ContextualResult;

			template< template< typename > class Contextual_, typename BindingContext >
			struct ContextualResult< Contextual_< BindingContext > >:
				public Contextual_< BindingContext >
			{
				template< typename Ctx >
				using Contextual = Contextual_< Ctx >;
			};


			template< typename BoundContextual >
			struct MakeAsContextual;

			template< template< typename > class Contextual, typename Ctx >
			struct MakeAsContextual< Contextual< Ctx > >
			{
				using r = AsContextual< Contextual >;
			};


			using contextualRecords = typename Context::GetTypeRegistry::r::template Find< annotation::IsContextualRecord >::r;
			using contextualTypes = typename contextualRecords::template Apply< ContextBinder >::r;
			using selectedContextualTypes = typename contextualTypes::template Select< Selector >::r;
		
		public:
			using r = typename selectedContextualTypes::template Apply< MakeAsContextual >::r;
		};


		template< typename Ctx >
		struct GetComponents;

		template< typename... CtxLevel >
		struct GetComponents< Context< CtxLevel... > >
		{
		private:
			template< typename CLevel >
			struct GetContainedTypes
			{
				using r = typename CLevel::Container::ContainedTypes::r;
			};


			template< typename Next, typename Result >
			struct MergeContainedTypes
			{
				using r = typename Result::template PrependAll< Next >::r;
			};


		public:
			using r = typename Reduce< MergeContainedTypes, TypeList<>, typename GetContainedTypes< CtxLevel >::r... >::r;
		};


		template< typename Context, typename Component, typename CurrentContains = void >
		struct FindOwningContext;

		template< typename CurrentLevel, typename Component, typename... OtherLevel >
		struct FindOwningContext< Context< CurrentLevel, OtherLevel... >,
								  Component,
								  typename std::enable_if< CurrentLevel::Container::template Contains< Component >::r >::type >
		{
			using r = Context< CurrentLevel, OtherLevel... >;
		};

		template< typename Component, typename DoesNotMatter >
		struct FindOwningContext< Context<>, Component, DoesNotMatter >
		{
			using r = void;
		};

		template< typename CurrentLevel, typename Component, typename... OtherLevel, typename DoesNotContain >
		struct FindOwningContext< Context< CurrentLevel, OtherLevel... >,
								  Component,
								  DoesNotContain >
		{
			static_assert( sizeof...( OtherLevel ), "Owning context of component not found" );

			using r = typename FindOwningContext< Context< OtherLevel... >, Component >::r;
		};


		template< typename Ctx, typename Interface >
		struct FindImplementors
		{
		private:
			template< typename Implementor >
			struct IsImplementor
			{
				enum { r = std::is_base_of< Interface, Implementor >::value };
			};

		public:
			using r = typename Ctx::GetComponents::r::template Select< IsImplementor >::r;
		};


		template< typename Dep >
		struct CreateLambda
		{
			template< typename Ctx >
			static
			auto
			create( Ctx* ctx )
				-> std::function< decltype( ctx->template resolve< Dep >() )() >
			{
				return [ctx] () -> decltype( ctx->template resolve< Dep >() ) { return ctx->template resolve< Dep >(); };
			}
		};


		template< typename Deps >
		struct GatherDeps;

		template< typename... Dep >
		struct GatherDeps< TypeList< Dep... > >
		{
		private:
			template< typename ToInject >
			struct CreateInjectLambda;
			
			template< typename ToInject >
			struct CreateInjectLambda< ToInject& >
			{
				using injectedType = InjectedReference< typename std::decay< ToInject >::type >;
				
				static
				std::function< injectedType() >
				create( ToInject& toInject )
				{
					return [&toInject] () -> injectedType { return injectedType( toInject ); };
				}
			};

			template< typename ToInject >
			struct CreateInjectLambda< ToInject&& >
			{
				using injectedType = InjectedValue< typename std::decay< ToInject >::type >;
				
				static
				std::function< injectedType() >
				create( ToInject&& toInject )
				{
					return [&toInject] () -> injectedType { return injectedType( std::move( toInject ) ); };
				}
			};


		public:
			template< typename Ctx, typename... ToInject >
			static
			auto
			gather( Ctx* ctx, ToInject&&... toInject )
				-> decltype( std::make_tuple(
									CreateLambda< Dep >::create( ctx )...,
									CreateInjectLambda< ToInject&& >::create( std::forward< ToInject >( toInject ) )... ) )
			{
				return std::make_tuple(
							CreateLambda< Dep >::create( ctx )...,
							CreateInjectLambda< ToInject&& >::create( std::forward< ToInject >( toInject ) )... );
			}
		};


		template< typename Dep >
		struct IsInjected;

		template< typename Type >
		struct IsInjected< InjectedReference< Type > >
		{
			enum { r = true };
		};

		template< typename Type >
		struct IsInjected< InjectedValue< Type > >
		{
			enum { r = true };
		};

		template< typename Dep >
		struct IsInjected
		{
			enum { r = false };
		};


		template< typename Ctx, typename ToRetrieve >
		struct CanRetrieve
		{
		private:
			using decayed = typename std::decay< ToRetrieve >::type;

		public:
			enum { r = std::is_lvalue_reference< ToRetrieve >::value &&
					   FindImplementors< Ctx, decayed >::r::Length::r > 0 };
		};


		template< typename Ctx, typename ToResolve >
		struct FindResolvers
		{
			private:
			using allResolvers = typename FindRegisteredType< Ctx, annotation::IsResolver >::r;

			template< typename Next, typename Result >
			struct AppendIfResolves
			{
				using r = typename std::conditional<
							Next::Resolver::template Selector< ToResolve >::r,
							typename Result::template Append< Next >::r,
							Result >::type;
			};


			template< typename Resolvers >
			struct SelectAble;

			template< typename... Resolver >
			struct SelectAble< TypeList< Resolver... > >
			{
				using r = typename Reduce<
							AppendIfResolves,
							TypeList<>,
							Resolver... >::r;
			};


		public:
			using r = typename SelectAble< allResolvers >::r;
		};


		template< typename Ctx, typename ToResolve >
		struct GetResolver
		{
			using r = typename FindResolvers< Ctx, ToResolve >::r::Head::r;
		};


		template< typename Ctx, typename ToResolve >
		struct HasResolver
		{
			enum { r = FindResolvers< Ctx, ToResolve >::r::Length::r > 0 };
		};


		template< typename Resolver, typename ToResolve, typename Deps >
		struct ResolverResolve;

		template< typename Resolver, typename ToResolve, typename... Dep >
		struct ResolverResolve< Resolver, ToResolve, TypeList< Dep... > >
		{
			template< typename Ctx >
			static
			ToResolve
			resolve( Ctx* ctx )
			{
				return Resolver::template resolve< ToResolve >(
						CreateLambda< Dep >::create( ctx )... );
			}

		};


		template< typename Ctx, typename ToResolve >
		struct CanResolve
		{
			enum { r = CanRetrieve< Ctx, ToResolve >::r ||
					   HasResolver< Ctx, ToResolve >::r };
		};

	}


	template<>
	struct Context<>
	{
	private:
		using This = Context<>;


	public:
		template< typename... NewType >
		using CreateChildContext = context::CreateChildContext< This, NewType... >;


		inline
		Context()
		{}

		inline
		~Context()
		{}
	};


	inline
	Context<>
	createNullContext()
	{
		return Context<>();
	}


	template< typename... NewType >
	struct CreateInitialContext
	{
		using r = Context< typename CreateContextLevel< NewType... >::r >;
	};


	template< typename CurrentLevel, typename... ParentLevel >
	struct Context< CurrentLevel, ParentLevel... >
	{

		template< typename... Level >
		friend struct Context;

	private:

		using This = Context< CurrentLevel, ParentLevel... >;


		Context< ParentLevel... >&	parent;
		CurrentLevel				currentLevel;
	

	public:

		using ParentContext = Context< ParentLevel... >;

		template< typename... NewType >
		using CreateChildContext = context::CreateChildContext< This, NewType... >;

		template< typename... NewType >
		using Enrich = context::Enrich< This, NewType... >;

		using GetTypeRegistry = context::GetTypeRegistry< This >;

		using GetComponents = context::GetComponents< This >;

		template< typename Interface >
		using FindImplementors = context::FindImplementors< This, Interface >;

		template< typename Component >
		using FindOwningContext = context::FindOwningContext< This, Component >;

		template< typename ToResolve >
		using CanResolve = context::CanResolve< This, ToResolve >;


		template< typename... ToInject >
		Context( ParentContext& parentContext, ToInject&&... toInject ):
		  parent( parentContext ),
		  currentLevel( context::GatherDeps<
		  					typename CurrentLevel::GetDependencies::r
		  						::template RemoveBy< context::IsInjected >::r >
		  							::gather( this, std::forward< ToInject >( toInject )... ) )
		{}

		Context( const This& other ):
		  parent( other.parent ),
		  currentLevel( other.currentLevel )
		{}

		Context( This&& other ):
		  parent( other.parent ),
		  currentLevel( std::move( other.currentLevel ) )
		{}

		This&
		operator = ( const This& other )
		{
			this->parent = other.parent;
			this->currentLevel = other.currentLevel;

			return *this;
		}

		This&
		operator = ( This&& other )
		{
			this->parent = other.parent;
			this->currentLevel = std::move( other.currentLevel );

			return *this;
		}


		CurrentLevel&
		getCurrentLevel()
		{
			return currentLevel;
		}

		template< typename ToResolve >
		typename std::enable_if<
			context::CanRetrieve< This, ToResolve >::r,
			ToResolve >::type
		resolve()
		{
			using decayed = typename std::decay< ToResolve >::type;
			using implementor = typename context::FindImplementors< This, decayed >::r::Head::r;

			return retrieve< implementor >();

			/*std::string					toResolveName = common::typeName< ToResolve >();
			configuration::Interfaces&	interfacesConfig = this->template retrieve< configuration::Interfaces >();

			if( interfacesConfig.isConfigured( toResolveName ) )
			{
				auto	configuredImplementors = context::FindConfiguredImplementors< ToResolve >::find( *this );
				auto	implementorIt = configuredImplementors.find(
											interfacesConfig.getConfiguration( toResolveName ).providedById );

				if( implementorIt == configuredImplementors.end() )
				{
					// TODO
					std::cout << "Implementor not found" << std::endl;
					throw std::runtime_error( "Implementor not found" );
				}
				else
				{
					return implementorIt->second;
				}
			}
			else
			{
				using implementors = typename This::template FindImplementors< ToResolve >::r;
				using implementor = typename implementors::Head::r;
				
				return this->template retrieve< implementor >();
			}*/
		}

		template< typename ToResolve >
		typename std::enable_if<
			context::HasResolver< This, ToResolve >::r,
			ToResolve >::type
		resolve()
		{
			using resolver = typename context::GetResolver< This, ToResolve >::r;
			using deps = typename resolver::template GetDependencies< ToResolve >::r;

			return context::ResolverResolve< resolver, ToResolve, deps >::resolve( this );
		}


	private:

		template< typename ToRetrieve >
		ToRetrieve&
		retrieve()
		{
			using owningContext = typename This::template FindOwningContext< ToRetrieve >::r;
				
			owningContext&	componentContext = this->template accessContext< owningContext >();
			return componentContext.getCurrentLevel().getContainer().template access< ToRetrieve >().get();
		}
		

		template< typename ToAccessContext >
		typename std::enable_if<
						std::is_same< This, ToAccessContext >::value,
						ToAccessContext >::type&
		accessContext()
		{
			return *this;
		}

		template< typename ToAccessContext >
		typename std::enable_if<
						std::is_same< This, ToAccessContext >::value == false,
						ToAccessContext >::type&
		accessContext()
		{
			return parent.template accessContext< ToAccessContext >();
		}

	};

}