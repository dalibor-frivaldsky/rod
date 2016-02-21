#pragma once


#include <rod/Annotation.hpp>
#include <rod/TypeList.hpp>




namespace rod {
namespace annotation
{

	template< typename... NewType >
	struct ExtendWith
	{
		using NewTypes = TypeList< NewType... >;
	};

	ROD_Annotation( ExtendWith )

	template< typename T, typename IsNotAnnotated = void >
	struct GetExtendWith;

	template< typename T >
	struct GetExtendWith< T, typename std::enable_if< HasExtendWithAnnotation< T >::r >::type >
	{
		using r = typename T::ExtendWith::NewTypes;
	};

	template< typename T, typename IsNotAnnotated >
	struct GetExtendWith
	{
		using r = TypeList<>;
	};
	
}}