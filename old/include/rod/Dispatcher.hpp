#pragma once


#include <rod/dispatch/Linear.hpp>




namespace rod
{

	template<
			typename Branches,
			typename BranchHandle,
			typename BranchPerformer,
			template< typename, typename, typename > class DispatcherType = dispatch::Linear >
		class Dispatcher
		{
		private:
			using DispatcherImpl = DispatcherType<
										Branches,
										BranchHandle,
										BranchPerformer >;


			DispatcherImpl	dispatcherImpl;

		public:
			template< typename Handle, typename... ToForward >
			void
			dispatch( const Handle& handle, ToForward&&... toForward )
			{
				dispatcherImpl.dispatch( handle, std::forward< ToForward >( toForward )... );
			}
		};
	
}