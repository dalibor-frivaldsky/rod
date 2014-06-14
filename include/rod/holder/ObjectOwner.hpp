#pragma once


#include <utility>

#include <rod/TypeList.hpp>




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct ObjectOwner
		{

		private:

			struct Container
			{
				Type	object;

				Container( Type&& object ):
				  object( std::forward< Type >( object ) )
				{}
			};

			Container*	container = nullptr;

		public:

			using Dependencies = TypeList<>;

			~ObjectOwner()
			{
				if( container )
				{
					delete container;
				}
			}

			void
			setupFrom( Type&& object )
			{
				container = new Container( std::forward< Type >( object ) );
			}

			Type*
			get()
			{
				return &container->object;
			}

		};
		
	}
	
}