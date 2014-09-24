#pragma once


#include <type_traits>

#include <rod/Annotation.hpp>
#include <rod/TypeList.hpp>




namespace rod
{

	namespace annotation
	{

		template< typename... Requirement >
		struct Requires
		{
			using Requirements = TypeList< Requirement... >;
		};

		ROD_Annotation( Requires )

		template< typename T, typename IsNotAnnotated = void >
		struct GetRequirements;

		template< typename T >
		struct GetRequirements< T, typename std::enable_if< HasRequiresAnnotation< T >::r >::type >
		{
			using r = typename T::Requires::Requirements;
		};

		template< typename T, typename IsNotAnnotated >
		struct GetRequirements
		{
			using r = TypeList<>;
		};

	}
	
}