#pragma once


#include <functional>
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

			
			ObjectOwner( std::function< InjectedValue< Type >() >& ref ):
			  object( std::move( ref().object ) )
			{}

			ObjectOwner( ObjectOwner< Type >&& other ):
			  object( std::move( other.object ) )
			{}

			ObjectOwner( const ObjectOwner< Type >& ) = delete;


			Type&
			get()
			{
				return object;
			}

		};
		
	}
	
}