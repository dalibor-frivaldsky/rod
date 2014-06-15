#pragma once


#include <utility>




namespace rod
{

	template< typename Type >
	struct InjectedValue
	{
		Type&&	object;

		InjectedValue( Type&& object ):
		  object( std::move( object ) )
		{}
	};
	
}