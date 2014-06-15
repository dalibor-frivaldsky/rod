#pragma once




namespace rod
{

	template< typename Type >
	struct InjectedReference
	{
		Type&	object;

		InjectedReference( Type& object ):
		  object( object )
		{}
	};
	
}