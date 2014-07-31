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

			ObjectReference( ObjectReference< Type >&& other ):
			  object( other.object )
			{}

			ObjectReference( const ObjectReference< Type >& ) = delete;


			Type&
			get()
			{
				return object;
			}

		};
		
	}
	
}