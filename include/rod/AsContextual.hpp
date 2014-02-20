#pragma once


#include <rod/Context.hpp>
#include <rod/annotation/ContextualRecord.hpp>




namespace rod
{

	template< template< typename > class ContextualType >
	struct AsContextual
	{
		using ContextualRecord = annotation::ContextualRecord;

		using Type = AsContextual< ContextualType >;

		template< typename Context >
		struct Bind
		{
			using r = ContextualType< Context >;
		};
	};
	
}