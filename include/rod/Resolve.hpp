#pragma once


#include <rod/ContextAccessor.hpp>




namespace rod
{

	template< typename ToResolve, typename Context >
	ToResolve
	resolve( Context& context )
	{
		return accessContext( context ).template resolve< ToResolve >();
	}
	
}