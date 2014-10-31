#pragma once


#include <rod/ContextAccessor.hpp>




namespace rod
{

	template< typename Context, typename Matcher >
	struct Find
	{
	private:
		using Types = typename ContextAccessor< Context >::Context::GetTypeRegistry::r;

	public:
		using r = typename Types::template Find< Matcher::template Match >::r;
	};
	
}