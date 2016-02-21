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

			
			ObjectReference( Type& ref ):
			  object( ref )
			{}

			ObjectReference( const ObjectReference< Type >& other ):
			  object( other.object )
			{}

			ObjectReference( ObjectReference< Type >&& other ):
			  object( other.object )
			{}

			ObjectReference< Type >&
			operator = ( const ObjectReference< Type >& other )
			{
				this->object = other.object;

				return *this;
			}

			ObjectReference< Type >&
			operator = ( ObjectReference< Type >&& other )
			{
				this->object = other.object;

				return *this;
			}

			
			Type&
			get()
			{
				return object;
			}

		};
		
	}
	
}