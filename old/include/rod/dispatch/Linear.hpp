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
				template< typename Handle, typename... ToForward >
				static
				void
				perform( const Handle&, ToForward&&... )
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
				template< typename Handle, typename... ToForward >
				static
				void
				perform( const Handle& handle, ToForward&&... toForward )
				{
					if( handle == BranchHandle::template handle< Branch >() )
					{
						BranchPerformer::template perform< Branch >(
								std::forward< ToForward >( toForward )... );
					}
					else
					{
						DispatchChain< BranchHandle, BranchPerformer, Rest... >
							::perform( handle, std::forward< ToForward >( toForward )... );
					}
				}
			};
			
		}


		template<
			typename Branches,
			typename BranchHandle,
			typename BranchPerformer >
		struct Linear;

		template<
			typename BranchHandle,
			typename BranchPerformer,
			typename... Branch >
		struct Linear< TypeList< Branch... >, BranchHandle, BranchPerformer >
		{
			template< typename Handle, typename... ToForward >
			void
			dispatch( const Handle& handle, ToForward&&... toForward )
			{
				detail::DispatchChain<
					BranchHandle,
					BranchPerformer,
					Branch... >
						::perform( handle, std::forward< ToForward >( toForward )... );
			}
		};
		
	}
	
}