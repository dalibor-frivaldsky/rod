#pragma once


#include <utility>

#include <rod/Bind.hpp>
#include <rod/ContextualAccessor.hpp>




namespace rod
{

	namespace dispatch
	{

		namespace linear
		{

			template<
				typename BranchHandle,
				typename BranchPerformer,
				typename... Branches >
			struct DispatchChain;

			template< typename BranchHandle, typename BranchPerformer >
			struct DispatchChain< BranchHandle, BranchPerformer >
			{
				template< typename Contextual, typename Handle, typename... ToPass >
				static
				void
				perform( Contextual*, const Handle&, ToPass&&... toPass )
				{
					//std::cout << "No handler for command \"" << command.getName() << "\"" << std::endl;
					// TODO throw
				}
			};

			template<
				typename BranchHandle,
				typename BranchPerformer,
				template< typename > class BranchDef, typename Ctx,
				typename... Rest >
			struct DispatchChain< BranchHandle, BranchPerformer, BranchDef< Ctx >, Rest... >
			{
			private:
				using branch = BranchDef< Ctx >;


			public:
				template< typename ParentContextual, typename Handle, typename... ToPass >
				static
				void
				perform( ParentContextual* parentContextual, const Handle& handle, ToPass&&... toPass )
				{
					if( handle == BranchHandle::template handle< branch >() )
					{
						BranchPerformer::template perform< BranchDef >(
								parentContextual,
								std::forward< ToPass >( toPass )... );
					}
					else
					{
						DispatchChain< BranchHandle, BranchPerformer, Rest... >
							::perform( parentContextual, handle, std::forward< ToPass >( toPass )... );
					}
				}
			};
			
		}


		template<
			typename ParentContextual,
			typename BranchHandle,
			typename BranchPerformer,
			typename BranchRecords >
		struct Linear;

		template<
			typename ParentContextual,
			typename BranchHandle,
			typename BranchPerformer,
			typename... BranchRecord >
		struct Linear< ParentContextual, BranchHandle, BranchPerformer, TypeList< BranchRecord... > >
		{
		private:
			ParentContextual*	parentContextual;


			template< typename BR >
			struct BindBranch
			{
				using r = typename Bind< ParentContextual, BR::template Contextual >::r;
			};


		public:
			Linear( ParentContextual* parentContextual ):
			  parentContextual( parentContextual )
			{}

			template< typename Handle, typename... ToPass >
			void
			dispatch( const Handle& handle, ToPass&&... toPass )
			{
				linear::DispatchChain<
					BranchHandle,
					BranchPerformer,
					typename BindBranch< BranchRecord >::r... >
						::perform( parentContextual, handle, std::forward< ToPass >( toPass )... );
			}
		};
		
	}
	
}