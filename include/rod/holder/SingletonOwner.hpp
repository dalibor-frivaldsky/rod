#pragma once


#include <rod/annotation/ConstructWith.hpp>




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct SingletonOwner
		{

			using Dependencies = typename annotation::GetConstructionArguments< Type >::r;


			Type	object;

			template< typename... Arg >
			SingletonOwner( Arg&... arg ):
			  object( arg()... )
			{}

			Type&
			get()
			{
				return object;
			}
		};
		
	}
	
}