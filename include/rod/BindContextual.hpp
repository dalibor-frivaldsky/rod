#pragma once


#include <utility>

#include <rod/ContextualAccessor.hpp>




namespace rod
{

	template< typename ParentContextual, template< typename > class ChildContextual >
	struct BindContextual
	{
	private:
		using accessor = ContextualAccessor< ParentContextual >;
		using parentCtx = typename accessor::Context;

	public:
		using r = ChildContextual< typename parentCtx::template CreateChildContext<>::r >;
	};
	
}