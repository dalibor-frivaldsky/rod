#pragma once


#include <utility>

#include <rod/Find.hpp>




namespace rod
{

	namespace dispatch
	{

		template<
			typename Context,
			template< typename, typename, typename, typename > class DispatcherType,
			typename Matcher,
			typename BranchHandle,
			typename BranchPerformer >
		class Dispatcher
		{
		private:
			using Branches = typename Find< Context, Matcher >::r;
			using DispatcherImpl = DispatcherType<
										Context,
										BranchHandle,
										BranchPerformer,
										Branches >;


			DispatcherImpl	dispatcherImpl;

		public:
			Dispatcher( Context& context ):
			  dispatcherImpl( context )
			{}


			template< typename Handle, typename... ToForward >
			void
			dispatch( const Handle& handle, ToForward&&... toForward )
			{
				dispatcherImpl.dispatch( handle, std::forward< ToForward >( toForward )... );
			}
		};
		
	}
	
}