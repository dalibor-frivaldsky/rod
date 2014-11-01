#pragma once


#include <functional>
#include <type_traits>
#include <utility>

#include <rod/ContextAccessor.hpp>
#include <rod/ContextLevel.hpp>
#include <rod/Find.hpp>
#include <rod/Injected.hpp>
#include <rod/TypeList.hpp>
#include <rod/annotation/Component.hpp>
#include <rod/annotation/ContextualRecord.hpp>
#include <rod/annotation/Resolver.hpp>
#include <rod/match/Annotation.hpp>
#include <rod/match/Component.hpp>
#include <rod/match/Interface.hpp>




namespace rod
{

	template< typename... ContextLevels >
	struct Context;


	namespace detail
	{

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
		struct FilterNonInjectedComponents
		{
		private:
			template< typename T >
			struct IsNotInjectedComponent;

			template< typename T >
			struct IsNotInjectedComponent< Injected< T > >
			{
				enum { r = true };
			};

			template< typename T >
			struct IsNotInjectedComponent
			{
				enum { r = annotation::IsComponent< T >::r == false };
			};


			template< typename T >
			struct IsComponent
			{
				enum { r = !IsNotInjectedComponent< T >::r };
			};

		public:
			using Components = typename TypeList< NewType... >::template Select< IsComponent >::r;
			using NotInjectedComponents = typename TypeList< NewType... >::template Select< IsNotInjectedComponent >::r;
		};


		template< typename Context, typename... NewType >
		struct Enrich;

		template< typename CurrentLevel, typename... ParentLevel, typename... NewType >
		struct Enrich< Context< CurrentLevel, ParentLevel... >, NewType... >
		{
		private:
			using Filtered = FilterNonInjectedComponents< NewType... >;
			using NonComponentEnriched = typename Filtered::NotInjectedComponents::template UnpackTo<
											CurrentLevel::template Enrich >::r::r;
			using Enriched = typename Filtered::Components::template UnpackTo1<
										OrderedLevelEnrich,
										Context< NonComponentEnriched, ParentLevel... > >::r::r;

		public:
			using r = Context< Enriched, ParentLevel... >;
		};


		template< typename Ctx, typename... NewType >
		struct CreateChildContext;

		template< typename... CtxLevel, typename... NewType >
		struct CreateChildContext< Context< CtxLevel... >, NewType... >
		{
			using r = typename Enrich<
						Context<
							typename CreateContextLevel<>::r,
							CtxLevel... >,
						NewType... >::r;
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


		template< typename Ctx, typename ToRetrieve >
		struct CanRetrieve
		{
		private:
			using Decayed = typename std::decay< ToRetrieve >::type;

		public:
			enum { r = std::is_lvalue_reference< ToRetrieve >::value &&
					   Find<
					   		Ctx,
					   		match::Component< match::Interface< Decayed > > >::r::Length::r > 0 };
		};


		template< typename Ctx, typename ToResolve >
		struct FindResolvers
		{
			private:
			using allResolvers = typename Find<
									Ctx,
									match::Annotation< annotation::IsResolver > >::r;

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
						ctx->template resolve< Dep >()... );
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
		friend class ContextAccessor< Context<> >;


	private:
		using This = Context<>;

		This&
		getContext()
		{
			return *this;
		}

		struct GetContext
		{
			using r = This;
		};


	public:
		template< typename... NewType >
		using CreateChildContext = detail::CreateChildContext< This, NewType... >;


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

		friend class ContextAccessor< Context< CurrentLevel, ParentLevel... > >;

	private:

		using This = Context< CurrentLevel, ParentLevel... >;


		This&
		getContext()
		{
			return *this;
		}

		struct GetContext
		{
			using r = This;
		};


		Context< ParentLevel... >&	parent;
		CurrentLevel				currentLevel;
	

	public:

		using ParentContext = Context< ParentLevel... >;

		template< typename... NewType >
		using CreateChildContext = detail::CreateChildContext< This, NewType... >;

		template< typename... NewType >
		using Enrich = detail::Enrich< This, NewType... >;

		using GetTypeRegistry = detail::GetTypeRegistry< This >;

		template< typename Component >
		using FindOwningContext = detail::FindOwningContext< This, Component >;

		template< typename ToResolve >
		using CanResolve = detail::CanResolve< This, ToResolve >;


		template< typename... ToInject >
		Context( ParentContext& parentContext, ToInject&&... toInject ):
		  parent( parentContext ),
		  currentLevel( *this, std::forward< ToInject >( toInject )... )
		{}

		Context( const This& ) = delete;
		Context( This&& ) = delete;

		This& operator = ( const This& ) = delete;
		This& operator = ( This&& ) = delete;


		CurrentLevel&
		getCurrentLevel()
		{
			return currentLevel;
		}

		template< typename ToResolve >
		typename std::enable_if<
			detail::CanRetrieve< This, ToResolve >::r,
			ToResolve >::type
		resolve()
		{
			using Decayed = typename std::decay< ToResolve >::type;
			using Implementer = typename Find<
									This,
									match::Component< match::Interface< Decayed > > >::r
										::Head::r;

			return retrieve< Implementer >();

			/*std::string					toResolveName = common::typeName< ToResolve >();
			configuration::Interfaces&	interfacesConfig = this->template retrieve< configuration::Interfaces >();

			if( interfacesConfig.isConfigured( toResolveName ) )
			{
				auto	configuredImplementors = detail::FindConfiguredImplementors< ToResolve >::find( *this );
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
			detail::HasResolver< This, ToResolve >::r,
			ToResolve >::type
		resolve()
		{
			using resolver = typename detail::GetResolver< This, ToResolve >::r;
			using deps = typename resolver::template GetDependencies< ToResolve >::r;

			return detail::ResolverResolve< resolver, ToResolve, deps >::resolve( this );
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