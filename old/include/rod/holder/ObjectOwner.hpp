#pragma once


#include <functional>
#include <type_traits>
#include <utility>

#include <rod/TypeList.hpp>
#include <rod/holder/InjectedValue.hpp>




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct ObjectOwner
		{

		private:
			Type	object;


		public:

			using Dependencies = TypeList< InjectedValue< Type > >;

			
			ObjectOwner( Type&& ref ):
			  object( std::move( ref ) )
			{}

			ObjectOwner( const ObjectOwner< Type >& ) = delete;
			ObjectOwner( ObjectOwner< Type >&& ) = delete;

			ObjectOwner< Type >& operator = ( const ObjectOwner< Type >& ) = delete;
			ObjectOwner< Type >& operator = ( ObjectOwner< Type >&& ) = delete;


			Type&
			get()
			{
				return object;
			}

		};
		
	}
	
}