#pragma once


#include <rod/dispatch/Dispatcher.hpp>
#include <rod/dispatch/Linear.hpp>




namespace rod
{

	template<
		typename Matcher,
		typename BranchHandle,
		typename BranchPerformer,
		template< typename, typename, typename, typename > class DispatcherType = dispatch::Linear,
		typename Context >
	dispatch::Dispatcher<
		Context,
		DispatcherType,
		Matcher,
		BranchHandle,
		BranchPerformer >
	dispatcher( Context& context )
	{
		return dispatch::Dispatcher<
					Context,
					DispatcherType,
					Matcher,
					BranchHandle,
					BranchPerformer >( context );
	}
	
}