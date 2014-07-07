#pragma once


#include <rod/dispatch/Dispatcher.hpp>
#include <rod/dispatch/Linear.hpp>




namespace rod
{

	template<
		template< typename > class Selector,
		typename BranchHandle,
		typename BranchPerformer,
		template< typename, typename, typename, typename > class DispatcherType = dispatch::Linear,
		typename ParentContextual >
	dispatch::Dispatcher<
		ParentContextual,
		DispatcherType,
		Selector,
		BranchHandle,
		BranchPerformer >
	dispatcher( ParentContextual* parentContextual )
	{
		return dispatch::Dispatcher<
					ParentContextual,
					DispatcherType,
					Selector,
					BranchHandle,
					BranchPerformer >( parentContextual );
	}
	
}