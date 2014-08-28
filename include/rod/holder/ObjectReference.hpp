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

			ObjectReference( const ObjectReference< Type >& other ):
			  object( other.object )
			{}

			ObjectReference( ObjectReference< Type >&& other ):
			  object( other.object )
			{}

			ObjectReference< Type >& operator = ( const ObjectReference< Type >& ) = delete;
			ObjectReference< Type >& operator = ( ObjectReference< Type >&& ) = delete;

			
			Type&
			get()
			{
				return object;
			}

		};
		
	}
	
}