#pragma once


#include <rod/common/MakeVoid.hpp>




namespace rod
{

	namespace annotation
	{

		struct ContextualRecord
		{};


		template< typename NotAnnotated, typename = void >
		struct HasContextualRecordAnnotation
		{
			enum { r = false };
		};

		template< typename Annotated >
		struct HasContextualRecordAnnotation< Annotated, typename common::MakeVoid< typename Annotated::ContextualRecord >::r >
		{
			enum { r = true };
		};


		template< typename Type >
		struct IsContextualRecord
		{
			enum { r = HasContextualRecordAnnotation< Type >::r };
		};
		
	}
	
}