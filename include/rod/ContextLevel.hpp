#pragma once


#include <type_traits>

#include <rod/Container.hpp>
#include <rod/Reduce.hpp>
#include <rod/TypeList.hpp>
#include <rod/TypeRegistry.hpp>
#include <rod/annotation/Component.hpp>




namespace rod
{

	template< typename TypeRegistry, typename Container >
	struct ContextLevel;


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
			struct ExtractTypeApplier
			{
				using r = typename ExtractType< Type >::r;
			};

		public:
			using r = typename TypeList< T... >::template Apply< ExtractTypeApplier >::r;
		};


		template< typename... Type >
		struct SelectComponents
		{
			using r = typename TypeList< Type... >::template Select< annotation::IsComponent >::r;
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


	template< typename TypeRegistry, typename Container >
	struct ContextLevel:
		public Container
	{
		using Registry = TypeRegistry;
	};
	
}