#pragma once


#include <rod/common/MakeVoid.hpp>




namespace rod
{

	namespace annotation
	{

		struct Component
		{};


		template< typename NotAnnotated, typename = void >
		struct HasComponentAnnotation
		{
			enum { r = false };
		};

		template< typename Annotated >
		struct HasComponentAnnotation< Annotated, typename common::MakeVoid< typename Annotated::Component >::r >
		{
			enum { r = true };
		};


		template< typename Type >
		struct IsComponent
		{
			enum { r = HasComponentAnnotation< Type >::r };
		};
		
	}
	
}