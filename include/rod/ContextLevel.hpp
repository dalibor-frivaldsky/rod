#pragma once


#include <type_traits>
#include <utility>

#include <rod/Container.hpp>
#include <rod/Reduce.hpp>
#include <rod/TypeList.hpp>
#include <rod/TypeRegistry.hpp>
#include <rod/annotation/Component.hpp>




namespace rod
{

	template< typename TypeRegistry, typename Container >
	struct ContextLevel;

	template< typename... Type >
	struct CreateContextLevel;


	namespace contextLevel
	{
		template< typename Type, typename IsComponent = void >
		struct ExtractType;
		
		template< typename Component >
		struct ExtractType< Component, typename std::enable_if< annotation::IsComponent< Component >::r >::type >
		{
			using r = typename Component::Type;
		} ;

		template< typename Type >
		struct ExtractType< Type, typename std::enable_if< !annotation::IsComponent< Type >::r >::type >
		{
			using r = Type;
		};
		

		template< typename... T >
		struct ExtractTypes
		{
		private:
			template< typename Type >
			struct Extract
			{
				using r = typename ExtractType< Type >::r;
			};

		public:
			using r = typename TypeList< T... >::template Apply< Extract >::r;
		};


		template< typename... Type >
		struct SelectComponents
		{
			using r = typename TypeList< Type... >::template Select< annotation::IsComponent >::r;
		};


		template< typename Level, typename... NewType >
		struct Enrich
		{
		private:
			using newLevel = typename CreateContextLevel< NewType... >::r;

		public:
			using r = ContextLevel<
							typename Level::Registry::template Merge< typename newLevel::Registry >::r,
							typename Level::Container::template Merge< typename newLevel::Container >::r
					  >;
		};
		
	}


	template< typename... Type >
	struct CreateContextLevel
	{
		using r = ContextLevel<
					typename contextLevel::ExtractTypes< Type... >::r
						::template UnpackTo< CreateTypeRegistry >::r::r,
					typename contextLevel::SelectComponents< Type... >::r
						::template UnpackTo< CreateContainer >::r::r
				  >;
	};


	template< typename TypeRegistry, typename Container_ >
	struct ContextLevel
	{
	private:

		using This = ContextLevel< TypeRegistry, Container_ >;

		Container_	container;


	public:

		using Registry = TypeRegistry;
		using Container = Container_;


		template< typename ArgTuple >
		ContextLevel( ArgTuple&& argTuple ):
		  container( std::forward< ArgTuple >( argTuple ) )
		{}

		ContextLevel( ContextLevel< Registry, Container >&& other ):
		  container( std::move( other.container ) )
		{}

		ContextLevel( const ContextLevel< Registry, Container >& ) = delete;


		template< typename... NewType >
		using Enrich = contextLevel::Enrich< This, NewType... >;

		using GetDependencies = typename Container::GetDependencies;

		Container_&
		getContainer()
		{
			return container;
		}
	};
	
}