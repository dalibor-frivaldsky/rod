#pragma once


#include <type_traits>

#include <rod/Annotation.hpp>
#include <rod/TypeList.hpp>




namespace rod
{

	namespace annotation
	{

		template< typename... Arg >
		struct ConstructWith
		{
			using Args = TypeList< Arg... >;
		};

		ROD_Annotation( ConstructWith )

		template< typename T, typename IsNotAnnotated = void >
		struct GetConstructionArguments;

		template< typename T >
		struct GetConstructionArguments< T, typename std::enable_if< HasConstructWithAnnotation< T >::r >::type >
		{
			using r = typename T::ConstructWith::Args;
		};

		template< typename T, typename IsNotAnnotated >
		struct GetConstructionArguments
		{
			using r = TypeList<>;
		};

	}
	
}