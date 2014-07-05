#pragma once


#include <rod/annotation/ContextualRecord.hpp>




namespace rod
{

	template< template< typename > class ContextualType >
	struct AsContextual
	{
		using ContextualRecord = annotation::ContextualRecord;

		template< typename Context >
		using Contextual = ContextualType< Context >;

		template< typename Context >
		struct Bind
		{
			using r = ContextualType< Context >;
		};
	};
	
}