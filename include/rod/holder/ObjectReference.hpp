#pragma once


#include <rod/TypeList.hpp>




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct ObjectReference
		{

			Type*	object = nullptr;

		public:

			using Dependencies = TypeList<>;

			void
			setupFrom( Type& object )
			{
				this->object = &object;
			}

			Type*
			get()
			{
				return object;
			}

		};
		
	}
	
}