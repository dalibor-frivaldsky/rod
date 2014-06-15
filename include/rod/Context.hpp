#pragma once


/*#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

#include <rod/AsEnrichment.hpp>
#include <rod/Container.hpp>
#include <rod/Generate.hpp>
#include <rod/TypeList.hpp>
#include <rod/annotation/AutoResolve.hpp>
#include <rod/annotation/Component.hpp>
#include <rod/annotation/ContextualRecord.hpp>
#include <rod/common/NullType.hpp>
#include <rod/common/SameType.hpp>
#include <rod/common/Select.hpp>
#include <rod/common/Sequence.hpp>
#include <rod/common/TypeName.hpp>
#include <rod/configuration/Components.hpp>
#include <rod/configuration/Interfaces.hpp>
#include <rod/debug/Inspect.hpp>
#include <rod/holder/ObjectOwner.hpp>
#include <rod/holder/ObjectReference.hpp>
#include <rod/holder/SingletonHolder.hpp>*/

#include <functional>
#include <type_traits>
#include <utility>

#include <rod/ContextLevel.hpp>
#include <rod/TypeList.hpp>
#include <rod/annotation/Component.hpp>
#include <rod/holder/InjectedReference.hpp>
#include <rod/holder/InjectedValue.hpp>




namespace rod
{

	template< typename... ContextLevels >
	struct Context;


	namespace context
	{

		template< typename ParentContext >
		struct ParentContextRef
		{
			ParentContext&	parentContext;

			ParentContextRef( ParentContext& parentContext ):
			  parentContext( parentContext )
			{}
		};


		template< typename Component, typename ExistingComponents >
		struct DependenciesSatisfied
		{
		private:
			using dependencies = typename Component::Holder::Dependencies;

		public:
			enum { r = ExistingComponents::template ContainsAll< dependencies >::r };
		};


		template< typename NewComponents, typename ExistingComponents >
		struct SelectCreatable;

		template< typename... NewComponent, typename ExistingComponents >
		struct SelectCreatable< TypeList< NewComponent... >, ExistingComponents >
		{
		private:
			template< typename NewComp, typename Satisfied >
			struct AppendIfStatisfied
			{
				using r = typename common::Select<
											DependenciesSatisfied< NewComp, ExistingComponents >::r,
											typename Satisfied::template Append< NewComp >::r,
											Satisfied >::r;
			};
		
		public:
			using r = typename Reduce< AppendIfStatisfied, TypeList<>, NewComponent... >::r;
		};


		template< typename NewComponents, typename ExistingComponents >
		struct SortComponentsByDependency;

		template< typename ExistingComponents >
		struct SortComponentsByDependency< TypeList<>, ExistingComponents >
		{
			using r = TypeList<>;
		};

		template< typename NewComponents, typename ExistingComponents >
		struct SortComponentsByDependency
		{
		private:
			template< typename Comp >
			struct ExtractType
			{
				using r = typename Comp::Type;
			};

			using creatable = typename SelectCreatable< NewComponents, ExistingComponents >::r;
			static_assert( creatable::Length::r > 0, "Unable to create all components - dependencies not met" );

			using mergedComponentSet = typename ExistingComponents::template AppendAll< typename creatable::template Apply< ExtractType >::r >::r;
			using remainingNewComponents = typename NewComponents::template RemoveList< creatable >::r;
			using nextCreatable = typename SortComponentsByDependency< remainingNewComponents, mergedComponentSet >::r;

		public:
			using r = typename creatable::template AppendAll< nextCreatable >::r;
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


		template< typename Ctx, typename... NewType >
		struct OrderByDependencies
		{
		private:
			using filter = FilterComponents< NewType... >;
			using sortedComponents = typename SortComponentsByDependency<
												typename filter::Components,
												typename Ctx::GetComponents::r >::r;

		public:
			using r = typename filter::NotComponents::template AppendAll< sortedComponents >::r;
		};


		template< typename Ctx, typename... NewType >
		struct CreateChildContext;

		template< typename... CtxLevel, typename... NewType >
		struct CreateChildContext< Context< CtxLevel... >, NewType... >
		{
			using r = Context<
						typename CreateContextLevel< NewType... >::r,
						CtxLevel... >;
		};


		template< typename Context, typename... NewType >
		struct Enrich;

		template< typename CurrentLevel, typename... ParentLevel, typename... NewType >
		struct Enrich< Context< CurrentLevel, ParentLevel... >, NewType... >
		{
			using r = Context<
						typename OrderByDependencies<
									Context< CurrentLevel, ParentLevel... >,
									NewType... >::r::template UnpackTo< CurrentLevel::template Enrich >::r::r,
						ParentLevel... >;
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


		template< typename Deps >
		struct GatherDeps;

		template< typename... Dep >
		struct GatherDeps< TypeList< Dep... > >
		{
		private:
			template< typename D >
			struct CreateLambda
			{
				template< typename Ctx >
				static
				std::function< D&() >
				create( Ctx* ctx )
				{
					return [ctx] () -> D& { return ctx->template resolve< D >(); };
				}
			};


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

	}


	template<>
	struct Context<>
	{
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
		friend class Context;

	private:

		using This = Context< CurrentLevel, ParentLevel... >;


		// TODO is the ParentContextRef still necessary?
		context::ParentContextRef< Context< ParentLevel... > >		parentRef;

		CurrentLevel		currentLevel;
	

	public:

		using ParentContext = Context< ParentLevel... >;

		template< typename... NewType >
		using CreateChildContext = context::CreateChildContext< This, NewType... >;

		template< typename... NewType >
		using Enrich = context::Enrich< This, NewType... >;

		using GetTypeRegistry = context::GetTypeRegistry< This >;

		template< template< typename > class Selector >
		using FindRegisteredType = context::FindRegisteredType< This, Selector >;

		using GetComponents = context::GetComponents< This >;

		template< typename Component >
		using FindOwningContext = context::FindOwningContext< This, Component >;


		template< typename... ToInject >
		Context( ParentContext& parentContext, ToInject&&... toInject ):
		  parentRef( parentContext ),
		  currentLevel( context::GatherDeps<
		  					typename CurrentLevel::GetDependencies::r
		  						::template RemoveBy< context::IsInjected >::r >
		  							::gather( this, std::forward< ToInject >( toInject )... ) )
		{}


		CurrentLevel&
		getCurrentLevel()
		{
			return currentLevel;
		}

		template< typename ToResolve >
		ToResolve&
		resolve()
		{
			return retrieve< ToResolve >();

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
			return parentRef.parentContext.template accessContext< ToAccessContext >();
		}

	};


	/*namespace context
	{

		struct RootContext
		{
			using ParentContext = RootContext;

			template< typename Components >
			struct CreateChildContext
			{
				using r = RootContext;
			};

			template< typename Type >
			struct Extend
			{
				using r = RootContext;
			};

			struct ContextTypeList
			{
				using r = TypeList<>;
			};

			template< typename ToFind >
			struct FindImplementors
			{
				using r = TypeList<>;
			};

			template< template< typename > class Selector >
			struct FindAnnotatedContextual
			{
				using r = TypeList<>;
			};

			template< template< typename > class Selector >
			struct FindAnnotated
			{
				using r = TypeList<>;
			};
		};


		template< typename TypeRegistry_, typename Container_ >
		struct ContextDef
		{
			using TypeRegistry = TypeRegistry_;
			using Container = Container_;
		};


		template< typename ToAccess, typename Context >
		auto
		access( Context& context ) -> decltype( context.getContainer().template access< ToAccess >() )
		{
			return context.getContainer().template access< ToAccess >();
		}


		template< typename ComponentDef >
		struct ExtractType
		{
			using r = typename ComponentDef::Type;
		};


		template< typename ParentDef, typename Components >
		struct Create
		{
		private:
			// TODO strip references/pointers and constness
			using extendedTypeRegistry = typename ParentDef::TypeRegistry::template AppendAll< typename Components::template Apply< ExtractType >::r >::r;

			using newComponentsContainer = typename CreateContainer< Components >::r;
			
		public:
			using r = ContextDef< extendedTypeRegistry, newComponentsContainer >;
		};


		template< typename Component, typename ExistingComponents >
		struct DependenciesSatisfied
		{
		private:
			using dependencies = typename annotation::GetAutoResolves< typename Component::Type >::r::Types;

		public:
			enum { r = ExistingComponents::template ContainsAll< dependencies >::r };
		};


		template< typename NewComponents, typename ExistingComponents >
		struct SelectCreatable;

		template< typename... NewComponents, typename... ExistingComponents >
		struct SelectCreatable< TypeList< NewComponents... >, TypeList< ExistingComponents... > >
		{
		private:
			template< typename NewComponent, typename Satisfied >
			struct AppendIfStatisfied
			{
				using r = typename common::Select<
											DependenciesSatisfied< NewComponent, TypeList< ExistingComponents... > >::r,
											typename Satisfied::template Append< NewComponent >::r,
											Satisfied >::r;
			};
		
		public:
			using r = typename Reduce< AppendIfStatisfied, TypeList<>, NewComponents... >::r;
		};


		template< typename NewComponents, typename ExistingComponents >
		struct SortComponentsByDependency;

		template< typename ExistingComponents >
		struct SortComponentsByDependency< TypeList<>, ExistingComponents >
		{
			using r = TypeList<>;
		};

		template< typename NewComponents, typename ExistingComponents >
		struct SortComponentsByDependency
		{
		private:
			using creatable = typename SelectCreatable< NewComponents, ExistingComponents >::r;
			static_assert( creatable::Length::r > 0, "Unable to create all components - dependencies not met" );

			using mergedComponentSet = typename ExistingComponents::template AppendAll< typename creatable::template Apply< ExtractType >::r >::r;
			using remainingNewComponents = typename NewComponents::template RemoveList< creatable >::r;
			using nextCreatable = typename SortComponentsByDependency< remainingNewComponents, mergedComponentSet >::r;

		public:
			using r = typename creatable::template AppendAll< nextCreatable >::r;
		};


		template< typename Def, typename AllComponents, typename NewTypes >
		struct Extend
		{
		private:
			template< typename ComponentDef >
			struct IsComponent
			{
				enum { r = annotation::IsComponent< typename ComponentDef::Type >::r };
			};

			using extendedTypeRegistry = typename Def::TypeRegistry::template AppendAll< typename NewTypes::template Apply< ExtractType >::r >::r;

			using componentTypes = typename NewTypes::template Select< IsComponent >::r;
			using sortedComponentTypes = typename SortComponentsByDependency< componentTypes, AllComponents >::r;
			using newTypesContainer = typename CreateContainer< sortedComponentTypes >::r;
			using extendedTypesContainer = typename Def::Container::template Merge< newTypesContainer >::r;
			
		public:
			using r = ContextDef< extendedTypeRegistry, extendedTypesContainer >;
		};


		template< typename Container, typename Interface >
		struct GetImplementors;

		template< typename Interface >
		struct GetImplementors< Container< common::NullType >, Interface >
		{
			using r = TypeList<>;
		};

		template< typename Container, typename Interface >
		struct GetImplementors
		{
			using r = typename Container::template FindImplementors< Interface >::r;
		};


		template< typename Context, template< typename > class Selector >
		struct FindAnnotatedContextual
		{
		private:

			template< typename ContextualRec >
			struct ContextBinder
			{
				using r = typename ContextualRec::template Bind< RootContext >::r;
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
			struct WrapInResult
			{
				using r = ContextualResult< BoundContextual >;
			};


			using contextualRecords = typename Context::TypeRegistry::template Select< annotation::IsContextualRecord >::r;
			using contextualTypes = typename contextualRecords::template Apply< ContextBinder >::r;
			using selectedContextualTypes = typename contextualTypes::template Select< Selector >::r;
			using contextuals = typename selectedContextualTypes:: template Apply< WrapInResult >::r;

			// TODO separate finding of contextuals and components
			using components = typename Context::TypeRegistry::template Select< Selector >::r;
		
		public:
			//using r = typename components::template AppendAll< selectedContextualTypes >::r;
			using r = typename components::template AppendAll< contextuals >::r;
		};


		template< typename Context, template< typename > class Selector >
		struct FindAnnotated
		{
			using r = typename Context::TypeRegistry::template Select< Selector >::r;
		};


		template< typename CurrentContext, typename Component, bool isCurrentContextOwner >
		struct FindOwningContext;

		template< typename CurrentContext, typename Component >
		struct FindOwningContext< CurrentContext, Component, true >
		{
			using r = CurrentContext;
		};

		template< typename CurrentContext, typename Component, bool isCurrentContextOwner >
		struct FindOwningContext
		{
			using r = typename CurrentContext::ParentContext::template FindOwningContext< Component >::r;
		};


		template< typename CurrentContext, typename ToObtainContext >
		struct ObtainContext;

		template< typename ToObtainContext >
		struct ObtainContext< ToObtainContext, ToObtainContext >
		{
			static
			ToObtainContext&
			obtain( ToObtainContext& context )
			{
				return context;
			}
		};

		template< typename CurrentContext, typename ToObtainContext >
		struct ObtainContext
		{
			static
			ToObtainContext&
			obtain( CurrentContext& context )
			{
				return context.getParentContext().template obtainContext< ToObtainContext >();
			}
		};


		template< typename ToCreateTypeList >
		struct CreateNewContextObjects;

		template<>
		struct CreateNewContextObjects< TypeList<> >
		{
			template< typename Context >
			static
			void
			create( Context& context )
			{}
		};

		template< typename ToCreateTypeList >
		struct CreateNewContextObjects
		{
		private:

			template< typename ObjectToCreate, typename Depenencies >
			struct CreateObject;

			template< typename ObjectToCreate, typename... Dependency >
			struct CreateObject< ObjectToCreate, TypeList< Dependency... > >
			{
				template< typename Context >
				static
				ObjectToCreate*
				create( Context& context )
				{
					return new ObjectToCreate( context.template resolve< Dependency >()... );
				}
			};


			template< typename ObjectToCreate, typename Context >
			static
			ObjectToCreate*
			createObject( Context& context )
			{
				using autoResolves = typename annotation::GetAutoResolves< ObjectToCreate >::r::Types;
				
				return CreateObject< ObjectToCreate, autoResolves >::create( context );
			}
			

		public:
			template< typename Context >
			static
			void
			create( Context& context )
			{
				using toCreate = typename ToCreateTypeList::Head::r;

				access< toCreate >( context ).setupFrom( createObject< toCreate >( context ) );

				CreateNewContextObjects< typename ToCreateTypeList::Tail::r >::create( context );
			}
		};


		template< typename Interface >
		struct FindConfiguredImplementors
		{
		private:
			template< typename Implementors >
			struct ExtractConfigured;

			template< typename... Implementors >
			struct ExtractConfigured< TypeList< Implementors... > >
			{
				using ImplementorMap = std::map< std::string, Interface& >;

				static
				int
				addToMap( ImplementorMap& map, const std::string& id, Interface& implementor )
				{
					if( id.empty() == false )
					{
						map.emplace( id, implementor );
					}

					return 0;
				}

				template< typename Implementor >
				static
				std::string
				getImplementorId( configuration::Components& componentConfig )
				{
					std::string		implementorName = common::typeName< Implementor >();

					if( componentConfig.isConfigured( implementorName ) )
					{
						return componentConfig.getConfiguration( implementorName ).id;
					}
					else
					{
						return "";
					}
				}

				template< typename Implementor, typename Context >
				static
				Implementor&
				getImplementor( Context& context )
				{
					return context.template retrieve< Implementor >();
				}

				template< typename Context >
				static
				std::map< std::string, Interface& >
				extract( Context& context )
				{
					std::map< std::string, Interface& >		configuredImplementors;

					generate
					{
						addToMap(
							configuredImplementors,
							getImplementorId< Implementors >( context.template retrieve< configuration::Components >() ),
							getImplementor< Implementors >( context ) )...
					};
					
					return configuredImplementors;
				}
			};

			

		public:
			template< typename Context >
			static
			std::map< std::string, Interface& >
			find( Context& context )
			{
				using allImplementors = typename Context::template FindImplementors< Interface >::r;

				return ExtractConfigured< allImplementors >::extract( context );
			}
		};

	}


	template< typename ParentContext, typename ContextDef >
	struct Context;


	struct CreateContext
	{
		using r = Context<
						context::RootContext,
						context::ContextDef<
								TypeList<>,
								typename CreateContainer< TypeList<> >::r > >;
	};


	template< typename ParentCtx, typename ContextDef >
	struct Context:
		public ContextDef
	{

	public:

		using ParentContext = ParentCtx;


	private:

		using This = Context< ParentCtx, ContextDef >;
		

		template< typename ParentC, typename ContextD >
		friend struct Context;


		ParentContext*				parentContext = nullptr;
		typename This::Container	container;
		

	public:

		struct ContextTypeList
		{
		private:
			using currentComponents = typename ContextDef::Container::ContainedTypes::r;
			using parentComponents = typename ParentContext::ContextTypeList::r;

		public:
			using r = typename parentComponents::template AppendAll< currentComponents >::r;
		};


		template< typename Components >
		struct CreateChildContext
		{
			using r = Context< This, typename context::Create< ContextDef, Components >::r >;
		};


		template< typename NewTypes >
		struct Extend
		{
			using r = Context<
							ParentContext,
							typename context::Extend<
										ContextDef,
										typename This::ContextTypeList::r,
										NewTypes >::r >;
		};

		template< template< typename > class Selector >
		using FindAnnotatedContextual = context::FindAnnotatedContextual< This, Selector >;

		template< template< typename > class Selector >
		using FindAnnotated = context::FindAnnotated< This, Selector >;

		template< typename Component >
		struct FindOwningContext
		{
		private:
			enum { amComponentOwner = This::Container::template Contains< Component >::r };

		public:
			using r = typename context::FindOwningContext< This, Component, amComponentOwner >::r;
		};

		template< typename ToFind >
		struct FindImplementors
		{
		private:
			using currentImplementors = typename context::GetImplementors< typename This::Container, ToFind >::r;
			using parentImplementors = typename ParentContext::template FindImplementors< ToFind >::r;

		public:
			using r = typename currentImplementors::template AppendAll< parentImplementors >::r;
		};

		
		Context() = default;


		ParentContext&
		getParentContext()
		{
			return *parentContext;
		}

		typename This::Container&
		getContainer()
		{
			return container;
		}

		const typename This::Container&
		getContainer() const
		{
			return container;
		}


		template< typename... Component >
		void
		injectComponents( Component&&... component )
		{
			generate
			(
				(context::access< typename std::decay< Component >::type >( *this ).setupFrom( std::forward< Component >( component ) ), 0)...
			);
		}

		void
		bindToParent( ParentContext& parentContext )
		{
			this->parentContext = &parentContext;
			
			using singletons = typename This::Container::template Select< holder::SingletonSelector >::r;
			context::CreateNewContextObjects< singletons >::create( *this );
		}

		template< typename ToObtainContext >
		ToObtainContext&
		obtainContext()
		{
			return context::ObtainContext< This, ToObtainContext >::obtain( *this );
		}

		template< typename ToRetrieve >
		ToRetrieve&
		retrieve()
		{
			using owningContext = typename This::template FindOwningContext< ToRetrieve >::r;
				
			owningContext&	implementorContext = this->template obtainContext< owningContext >();
			return *context::access< ToRetrieve >( implementorContext ).get();
		}

		template< typename ToResolve >
		ToResolve&
		resolve()
		{
			std::string					toResolveName = common::typeName< ToResolve >();
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
			}
		}


	private:

		Context( const This& other );
		Context( This&& other );

		This&
		operator = ( const This& other );

	};*/

}