#pragma once


#include <utility>

#include <rod/TypeList.hpp>




namespace rod
{

	namespace dispatch
	{

		namespace detail
		{

			template<
				typename BranchHandle,
				typename BranchPerformer,
				typename... Branch >
			struct DispatchChain;

			template< typename BranchHandle, typename BranchPerformer >
			struct DispatchChain< BranchHandle, BranchPerformer >
			{
				template< typename Context, typename Handle, typename... ToForward >
				static
				void
				perform( Context&, const Handle&, ToForward&&... )
				{
					//std::cout << "No handler for command \"" << command.getName() << "\"" << std::endl;
					// TODO throw
				}
			};

			template<
				typename BranchHandle,
				typename BranchPerformer,
				typename Branch,
				typename... Rest >
			struct DispatchChain< BranchHandle, BranchPerformer, Branch, Rest... >
			{
			public:
				template< typename Context, typename Handle, typename... ToForward >
				static
				void
				perform( Context& context, const Handle& handle, ToForward&&... toForward )
				{
					if( handle == BranchHandle::template handle< Branch >() )
					{
						BranchPerformer::template perform< Branch >(
								context,
								std::forward< ToForward >( toForward )... );
					}
					else
					{
						DispatchChain< BranchHandle, BranchPerformer, Rest... >
							::perform( context, handle, std::forward< ToForward >( toForward )... );
					}
				}
			};
			
		}


		template<
			typename Context,
			typename BranchHandle,
			typename BranchPerformer,
			typename Branches >
		struct Linear;

		template<
			typename Context,
			typename BranchHandle,
			typename BranchPerformer,
			typename... Branch >
		struct Linear< Context, BranchHandle, BranchPerformer, TypeList< Branch... > >
		{
		private:
			Context&	context;


		public:
			Linear( Context& context ):
			  context( context )
			{}

			template< typename Handle, typename... ToForward >
			void
			dispatch( const Handle& handle, ToForward&&... toForward )
			{
				detail::DispatchChain<
					BranchHandle,
					BranchPerformer,
					Branch... >
						::perform( context, handle, std::forward< ToForward >( toForward )... );
			}
		};
		
	}
	
}