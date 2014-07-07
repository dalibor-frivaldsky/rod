#pragma once


#include <utility>

#include <rod/ContextualAccessor.hpp>




namespace rod
{

	namespace dispatch
	{

		template<
			typename ParentContextual,
			template< typename, typename, typename, typename > class DispatcherType,
			template< typename > class Selector,
			typename BranchHandle,
			typename BranchPerformer >
		class Dispatcher
		{
		private:
			using Accessor = ContextualAccessor< ParentContextual >;
			using DispatcherImpl = DispatcherType<
										ParentContextual,
										BranchHandle,
										BranchPerformer,
										typename Accessor::Context
											::template FindRegisteredContextual< Selector >::r >;


			DispatcherImpl	dispatcherImpl;

		public:
			Dispatcher( ParentContextual* parentContextual ):
			  dispatcherImpl( parentContextual )
			{}


			template< typename Handle, typename... ToPass >
			void
			dispatch( const Handle& handle, ToPass&&... toPass )
			{
				dispatcherImpl.dispatch( handle, std::forward< ToPass >( toPass )... );
			}
		};
		
	}
	
}