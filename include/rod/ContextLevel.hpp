#pragma once


#include <tuple>
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


	namespace detail
	{
		template< typename Type, typename IsComponent = void >
		struct TypeFromComponent;
		
		template< typename Component >
		struct TypeFromComponent< Component, typename std::enable_if< annotation::IsComponent< Component >::r >::type >
		{
			using r = typename Component::Type;
		} ;

		template< typename Type >
		struct TypeFromComponent< Type, typename std::enable_if< !annotation::IsComponent< Type >::r >::type >
		{
			using r = Type;
		};
		

		template< typename... T >
		struct TypeFromComponents
		{
		private:
			template< typename Type >
			struct Extract
			{
				using r = typename TypeFromComponent< Type >::r;
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
					typename detail::TypeFromComponents< Type... >::r
						::template UnpackTo< CreateTypeRegistry >::r::r,
					typename detail::SelectComponents< Type... >::r
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


		template< typename Context, typename... ToInject >
		ContextLevel( Context& context, ToInject&&... toInject ):
		  container( context, std::forward_as_tuple( std::forward< ToInject >( toInject )... ) )
		{}

		ContextLevel( const This& ) = delete;
		ContextLevel( This&& ) = delete;

		This& operator = ( const This& ) = delete;
		This& operator = ( This&& ) = delete;


		template< typename... NewType >
		using Enrich = detail::Enrich< This, NewType... >;

		Container_&
		getContainer()
		{
			return container;
		}
	};
	
}