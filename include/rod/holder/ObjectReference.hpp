#pragma once


#include <functional>

#include <rod/TypeList.hpp>
#include <rod/holder/InjectedReference.hpp>




namespace rod
{

	namespace holder
	{

		template< typename Type >
		class ObjectReference
		{

		private:
			Type&	object;


		public:

			using Dependencies = TypeList< InjectedReference< Type > >;

			
			ObjectReference( std::function< InjectedReference< Type >() >& ref ):
			  object( ref().object )
			{}

			Type&
			get()
			{
				return object;
			}

		};
		
	}
	
}